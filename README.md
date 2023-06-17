# Barebones HTTP Server in C
### Features:
- Specify port the socket is bound on as a command line argument
- Specify a root directory (with an index.html) to serve html to the client
- Error checking the socket(), bind(), listen(), accept(), and send() socket functions
- Request queue for multiple connections (not concurrent)
- Accepts GET and HEAD requests (other HTTP requests may come in the future)

### Usage:
1. Compile server.c with gcc ```$ gcc server.c -o server ```
2. Run executable with correct command line arguments ```$ server [PORT] [ROOT_DIR] ```
3. Test by sending an HTTP GET request (using postman, curl, chrome, etc.)
4. Check stdout for useful information about request. 
