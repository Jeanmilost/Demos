using System;
using System.Collections.Generic;
using System.Text;

namespace SimpleGameAssetInterface.Common
{
    /// <summary>
    /// Image helper functions.
    /// </summary>
    internal class ImageHelper
    {
        /// <summary>
        /// Provides a mapping dictionary to match the exact hexadecimal magic numbers to image MIME types.
        /// </summary>
        private static readonly Dictionary<byte[], string> ImageSignatures = new Dictionary<byte[], string>
        {
            { new byte[] { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A }, "image/png" },
            { new byte[] { 0xFF, 0xD8, 0xFF },                               "image/jpg" },    // matches both .jpg and .jpeg
            { new byte[] { 0x47, 0x49, 0x46, 0x38 },                         "image/gif" },
            { new byte[] { 0x42, 0x4D },                                     "image/bmp" },
            { new byte[] { 0x00, 0x00, 0x01, 0x00 },                         "image/x-icon" }, // matches .ico files
            { new byte[] { 0x52, 0x49, 0x46, 0x46 },                         "image/webp" }    // webp files (starts with 'RIFF' container header)
        };

        /// <summary>
        /// Gets the image mime type from a byte array containing the image data.
        /// </summary>
        /// <param name="fileBytes">The image file bytes from which the mime type should be extracted.</param>
        /// <returns>The image mime type, default application/octet-stream if not found or on error.</returns>
        public static string GetImageMimeType(byte[] fileBytes)
        {
            if (fileBytes == null || fileBytes.Length == 0)
                // generic binary fallback
                return "application/octet-stream";

            // check the byte array against our dictionary of known image magic numbers
            foreach (var signature in ImageSignatures)
                // ensure the source array is at least as long as the signature we are checking
                if (fileBytes.Length >= signature.Key.Length && fileBytes.Take(signature.Key.Length).SequenceEqual(signature.Key))
                    return signature.Value;

            // default fallback if not recognized as a known image
            return "application/octet-stream";
        }

        /// <summary>
        /// Validates the artwork data mime type.
        /// </summary>
        /// <param name="mime">Artwork mime type to validate.</param>
        /// <returns>Validated mime type.</returns>
        /// <exception cref="ArgumentException">Thrown if the mime type is unknown.</exception>
        public static string ValidateMimeType(string mime)
        {
            // check for known mime type
            if (mime != "image/png" &&
                mime != "image/jpg" &&
                mime != "image/jpeg" &&
                mime != "image/webp" &&
                mime != "image/bmp" &&
                mime != "image/gif" &&
                mime != "image/x-icon")
                throw new ArgumentException("Unknown mime type", nameof(mime));

            return mime;
        }
    }
}
