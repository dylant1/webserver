# Barebones HTTP Server in C
### Features:
- Specify port the socket is bound on as a command line argument
- Specify a root directory (with an index.html) to serve html to the client
- Error checking the socket(), bind(), listen(), accept(), and send() socket functions
- Request queue for multiple connections (not concurrent)
- Accepts GET and HEAD requests (other HTTP requests may come in the future)
