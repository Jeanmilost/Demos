/****************************************************************************
 * ==> GameImageAsset ------------------------------------------------------*
 ****************************************************************************
 * Description: Generic container for a game image asset.                   *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

using System.IO;
using System.Windows.Media.Imaging;
using System.Diagnostics;
using SimpleGameAssetInterface.Common;

namespace SimpleGameAssetInterface.Core
{
    /// <summary>
    /// Game image asset.
    /// </summary>
    public class GameImageAsset
    {
        protected byte[] m_Data     = [];
        protected string m_MimeType = string.Empty;

        /// <summary>
        /// The image asset data.
        /// </summary>
        public byte[] Data
        {
            get => m_Data;
            set => m_Data = value ?? [];
        }

        /// <summary>
        /// The image asset mime type.
        /// </summary>
        /// <exception cref="ArgumentException">The mime type cannot be unknown.</exception>
        public string MimeType
        {
            get => m_MimeType;
            set => m_MimeType = ImageHelper.ValidateMimeType(value);
        }

        public GameImageAsset()
        {}

        /// <summary>
        /// Loads an image content from a local file.
        /// </summary>
        /// <param name="fileName">Image file name to load.</param>
        /// <returns>true on success, otherwise false.</returns>
        public bool Load(string fileName)
        {
            try
            {
                if (!File.Exists(fileName))
                {
                    Trace.TraceWarning("Load image asset - file not found - {0}", fileName);
                    return false;
                }

                // read all bytes from the file
                byte[] rawData = File.ReadAllBytes(fileName);

                // populate the image data and mime type
                MimeType = ImageHelper.GetImageMimeType(rawData);
                Data     = rawData;
            }
            catch (Exception ex)
            {
                Trace.TraceError("Exception while loading file - {0} - error - {1}", fileName, ex.Message);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Loads an image content from an existing bitmap.
        /// </summary>
        /// <param name="bitmap">Source bitmap from which the image should be load.</param>
        /// <returns>true on success, otherwise false.</returns>
        public bool Load(BitmapImage bitmap)
        {
            try
            {
                // empty bitmap?
                if (bitmap == null)
                {
                    Trace.TraceWarning("Load image asset - source bitmap is empty");
                    return false;
                }

                // reading from an URI?
                if (bitmap.UriSource != null)
                {
                    var streamInfo = System.Windows.Application.GetResourceStream(bitmap.UriSource);

                    // open a stream from URI
                    if (streamInfo != null)
                        using (var stream = streamInfo.Stream)
                        {
                            // read the entire file into the byte array
                            byte[] streamData = new byte[stream.Length];
                            stream.ReadExactly(streamData);

                            MimeType = ImageHelper.GetImageMimeType(streamData);
                            Data     = streamData;

                            return true;
                        }
                }

                // no bitmap content?
                if (bitmap.PixelWidth == 0 || bitmap.PixelHeight == 0)
                {
                    Trace.TraceError("Load image asset - bitmap pixels are not loaded yet");
                    return false;
                }

                // re-encode the bitmap into a binary file
                using (var memoryStream = new MemoryStream())
                {
                    // use universal encoder, JpegBitmapEncoder will preserve the compression and the magic numbers
                    var encoder = new JpegBitmapEncoder();
                    encoder.Frames.Add(BitmapFrame.Create(bitmap));
                    encoder.Save(memoryStream);

                    byte[] fileData = memoryStream.ToArray();

                    // now the file header is rebuilt, the mime type may be deduced again
                    MimeType = ImageHelper.GetImageMimeType(fileData);
                    Data     = fileData;
                }
            }
            catch (Exception ex)
            {
                Trace.TraceError("Exception while loading file - error - {0}", ex.Message);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Gets a bitmap ready to be shown on a WPF interface.
        /// </summary>
        /// <returns>WPF compatible bitmap, null on error.</returns>
        public BitmapImage? GetBitmap()
        {
            try
            {
                if (m_Data == null || m_Data.Length == 0)
                {
                    Trace.TraceWarning("Get bitmap - image is empty");
                    return null;
                }

                BitmapImage bitmap = new();

                // convert the byte array back to a BitmapImage for the WPF UI
                using (MemoryStream stream = new(m_Data))
                {
                    bitmap.BeginInit();

                    // as bitmap is targeted to be used in WPF, forces WPF to load the image into memory immediately, preventing
                    // any file locking or stream access issues later
                    bitmap.CacheOption  = BitmapCacheOption.OnLoad;
                    bitmap.StreamSource = stream;
                    bitmap.EndInit();
                }

                // freeze the bitmap for performance optimization (optional but recommended)
                bitmap.Freeze();

                // return the newly created bitmap
                return bitmap;
            }
            catch (Exception ex)
            {
                Trace.TraceError("Exception while getting bitmap - error - {0}", ex.Message);
            }

            return null;
        }
    }
}
