/***********************************************************
 * Class to read cover artworks into files                 *
 * @Author - Jean-Milost Reymond                           *
 ***********************************************************/

using System;
using System.IO;
using System.Drawing;

namespace Player
{
    class ArtworkReader
    {
        #region Class variables
        
        private const int m_CoverFront = 0x03; // cover front value
        private Bitmap    m_Artwork;           // contains artwork, if exists

        #endregion

        #region Functions

        #region private functions

        /// <summary>
        /// Convert byte array to long integer
        /// </summary>
        /// <param name="array">Array of bytes to convert</param>
        /// <param name="size">Size of array to convert</param>
        /// <returns>Long integer converted value</returns>
        private long ConvertToLong(byte[] array, int size)
        {
            if (size > 8)
                throw new Exception("Array of byte is too big to be converted");

            long result = 0;

            // convert byte array to long integer
            for (int i = 0; i < size; ++i)
                result += (long)(array[size - i - 1] * Math.Pow(256, i));

            return result;
        }

        /// <summary>
        /// Convert byte array to synch safe long integer
        /// </summary>
        /// <param name="array">Array of bytes to convert</param>
        /// <param name="size">Size of array to convert</param>
        /// <returns>Long integer converted value</returns>
        private long ConvertToSynchSafe(byte[] array, int size)
        {
            if (size > 9)
                throw new Exception("Array of byte is too big to be converted");

            long result = 0;

            // convert byte array to long integer
            for (int i = 0; i < size; ++i)
                result += (long)((array[size - i - 1] & 127) * Math.Pow(128, i));

            return result;
        }

        /// <summary>
        /// Read artwork picture from MP3 file
        /// </summary>
        /// <param name="file">Stream containing file to read</param>
        /// <param name="tagSize">Size of tag containing picture</param>
        /// <param name="isID3v2">If true, MP3 contains ID3v2.2 tags</param>
        /// <returns>true if success, otherwise false</returns>
        private bool ReadPicture(Stream file, long tagSize, bool isID3v2)
        {
            try
            {
                // build text encoding container
                byte[] textEnc = new byte[1];

                // read text encoding
                file.Read(textEnc, 0, 1);

                int mimeLength = 0;

                // check if format is ID3v2
                if (isID3v2)
                {
                    // if yes, build container for image type
                    byte[] imageType = new byte[3];

                    // read image type
                    file.Read(imageType, 0, 3);

                    mimeLength = 3;
                }
                else
                {
                    // build image mime type container
                    byte[] mimeChar = new byte[1];
                    string mimeType = "";

                    do
                    {
                        // because we don't know total length of mime type description,
                        // we read it byte per byte, and we count number of chars
                        ++mimeLength;
                        file.Read(mimeChar, 0, 1);
                        mimeType += (char)mimeChar[0];
                    }
                    while (mimeChar[0] != 0);
                }

                // build container for picture type
                byte[] picType = new byte[1];

                // read picture type
                file.Read(picType, 0, 1);

                // if picture type is not "Cover Front", seek to next tag and quit
                if (picType[0] != m_CoverFront)
                {
                    file.Seek(tagSize - mimeLength - 2, SeekOrigin.Current);
                    return false;
                }

                // prepare container for description chars
                byte[] descChar = new byte[1];
                int descLength = 0;

                do
                {
                    // because we don't know description length, we read
                    // until next end mark char is reached, and we count number of chars
                    ++descLength;
                    file.Read(descChar, 0, 1);
                }
                while (descChar[0] != 0);

                // calculate size of picture raw data
                long pictureSize = tagSize - mimeLength - descLength - 2;

                // build picture raw data container
                byte[] picture = new byte[pictureSize];

                // read picture from file
                file.Read(picture, 0, (int)pictureSize);

                // build memory stream containing picture raw data
                Stream memoryPic = new MemoryStream(picture);

                // create artwork bitmap
                m_Artwork = new Bitmap(memoryPic);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        #endregion

        #region Public functions

        /// <summary>
        /// Open MP3 file and attempt to extract artwork
        /// </summary>
        /// <param name="fileName">File name containing MP3 data</param>
        /// <returns>true if success, otherwise false</returns>
        public bool Open(string fileName)
        {
            Stream file;

            try
            {
                // check if file exists
                if (!File.Exists(fileName))
                    return false;

                // open stream from file
                file = new FileStream(fileName, FileMode.Open);
            }
            catch (Exception)
            {
                return false;
            }

            try
            {
                // build MP3 tags signature container
                byte[] signature = new byte[3];

                // read ID3 signature into file. ID3 signature shold be first 3 bytes into file
                file.Read(signature, 0, 3);

                // check if signature is valid
                if (signature[0] != 'I' && signature[1] != 'D' && signature[2] != '3')
                    return false;

                // build ID3 version container
                byte[] version = new byte[2];

                // read ID3 version. ID3 version should be 2 next bytes after signature
                file.Read(version, 0, 2);

                // ID3 major version should be between 2.x and 4.x
                if (version[0] < 2 || version[0] > 4)
                    return false;

                // build ID3 flags container
                byte[] flags = new byte[1];

                // read ID3 flags. ID3 flags should be 1 next byte after version
                file.Read(flags, 0, 1);

                // build ID3 size container
                byte[] size = new byte[4];

                // read ID3 size. ID3 size should be 4 next bytes after version
                file.Read(size, 0, 4);

                // convert array of byte to synch safe long integer
                long sizeID3 = ConvertToSynchSafe(size, 4);

                // check if size is valid
                if (sizeID3 == 0)
                    return false;

                // check version of ID3 tags
                switch (version[0])
                {
                    // version is ID3v2.2
                    case 2:
                    {
                        // iterate throught file
                        for (long i = 0; i < sizeID3; )
                        {
                            // build containers for tag signature and size
                            byte[] headerSignature = new byte[3];
                            byte[] headerSize = new byte[3];

                            // read tag signature and size
                            file.Read(headerSignature, 0, 3);
                            file.Read(headerSize, 0, 3);

                            // convert byte array to long integer
                            long hSize = ConvertToLong(headerSize, 3);

                            // check if artwork signature was found
                            if (headerSignature[0] == 'P' && headerSignature[1] == 'I' &&
                                headerSignature[2] == 'C')
                            {
                                // attempt to read picture
                                if (ReadPicture(file, hSize, true))
                                    return true;
                            }
                            else
                                // otherwise seek file to next tag
                                file.Seek(hSize, SeekOrigin.Current);

                            // go to next tag
                            i += hSize;
                        }

                        break;
                    }

                    // version is ID3v2.3
                    case 3:
                    {
                        // iterate throught file
                        for (long i = 0; i < sizeID3; )
                        {
                            // build containers for tag signature, size and flags
                            byte[] headerSignature = new byte[4];
                            byte[] headerSize = new byte[4];
                            byte[] headerFlags = new byte[2];

                            // read tag signature, size and flags
                            file.Read(headerSignature, 0, 4);
                            file.Read(headerSize, 0, 4);
                            file.Read(headerFlags, 0, 2);

                            // convert byte array to long integer
                            long hSize = ConvertToLong(headerSize, 4);

                            // check if artwork signature was found
                            if (headerSignature[0] == 'A' && headerSignature[1] == 'P' &&
                                headerSignature[2] == 'I' && headerSignature[3] == 'C')
                            {
                                // attempt to read picture
                                if (ReadPicture(file, hSize, false))
                                    return true;
                            }
                            else
                                // otherwise seek file to next tag
                                file.Seek(hSize, SeekOrigin.Current);

                            // go to next tag
                            i += hSize;
                        }

                        break;
                    }

                    // version is ID3v2.4
                    case 4:
                    {
                        // iterate throught file
                        for (long i = 0; i < sizeID3; )
                        {
                            // build containers for tag signature, size and flags
                            byte[] headerSignature = new byte[4];
                            byte[] headerSize = new byte[4];
                            byte[] headerFlags = new byte[2];

                            // read tag signature, size and flags
                            file.Read(headerSignature, 0, 4);
                            file.Read(headerSize, 0, 4);
                            file.Read(headerFlags, 0, 2);

                            // convert byte array to long integer
                            long hSize = ConvertToSynchSafe(headerSize, 4);

                            // check if artwork signature was found
                            if (headerSignature[0] == 'A' && headerSignature[1] == 'P' &&
                                headerSignature[2] == 'I' && headerSignature[3] == 'C')
                            {
                                // attempt to read picture
                                if (ReadPicture(file, hSize, false))
                                    return true;
                            }
                            else
                                // otherwise seek file to next tag
                                file.Seek(hSize, SeekOrigin.Current);

                            // go to next tag
                            i += hSize;
                        }

                        break;
                    }

                    default:
                        return false;
                }

                return false;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                file.Close();
            }
        }

        /// <summary>
        /// Get bitmap containing artwork
        /// </summary>
        /// <returns>Bitmap containing artwork</returns>
        public Bitmap GetArtwork()
        {
            return m_Artwork;
        }

        #endregion

        #endregion
    }
}
