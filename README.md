# README

### Brief
A simple HTTP/2 cleartext server made with C++. The purpose of this project is to help myself learn more about the web, its HTTP protocol, and review C++. I do not intend this to be production ready at all and this will take some time to get working.

### Notes:
 - Only works on Unix-y systems because of the GNU Makefile.
 - Create the bin and build folder at the project root for the build to work.
 - Files in mains with names such as `test_*` are the unit tests.

### Todos:
 1. ~~Make special collections: BitArray, Prefix BT~~
 2. HPACK Huffman encoder & decoder.
 3. HPACK string and integer encoder & decoder.
 4. Make HPACK context class.
 5. Make HTTP/2 streams and other utils.
 6. Create server workers.
 7. Create server driver.
 8. Put together thread pool implementation.
 9. Finish up driver class of server.
 10. Test with cURL.
