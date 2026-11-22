from http.server import ThreadingHTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
import os

HOST = "127.0.0.1"
PORT = 8000

# Serve the UI files from the folder containing this script.
os.chdir(Path(__file__).resolve().parent)

class NoCacheHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header("Cache-Control", "no-store")
        super().end_headers()

if __name__ == "__main__":
    server = ThreadingHTTPServer((HOST, PORT), NoCacheHandler)
    print(f"HostelSync is running at http://localhost:{PORT}")
    print("Keep this terminal open. Press Ctrl+C to stop the server.")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nStopping HostelSync server...")
        server.server_close()
