# README

### Brief
A simple HTTP/2 cleartext server made with C++. The purpose of this project is to help myself learn more about the web, its HTTP protocol, and review C++. I do not intend this to be production ready at all and this will take some time to get working.

### WIP Screenshots

<img width="500" src="./imgs/H2Plus_Server_HPACK_Test1.png">

### Notes
 - Only works on Unix-y systems because of the GNU Makefile.
 - Create the bin and build folder at the project root for the build to work.
 - Files in mains with names such as `test_*` are the unit tests.

### Todos
 1. ~~Make special collections: BitArray, Prefix BT~~
 2. ~~HPACK Huffman encoder & decoder.~~
 3. _HPACK header indexing table (static, dynamic)._
 4. HPACK string and integer encoder & decoder.
 5. Make HPACK context class.
 6. Make HTTP/2 streams and other utils.
 7. Create server workers.
 8. Create server driver.
 9. Put together thread pool implementation.
 10. Finish up driver class of server.
 11. Test with cURL.
