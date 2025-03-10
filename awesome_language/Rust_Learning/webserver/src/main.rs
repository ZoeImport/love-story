use std::{
    fs, io::{Read, Write}, net::{TcpListener, TcpStream}, thread, time::Duration
};

use webserver::util::ThreadPool;
use simple_logger::SimpleLogger;
fn main() {
    SimpleLogger::new().init().unwrap();
    let listener = TcpListener::bind("127.0.0.1:9005").unwrap();
    log::info!("listen begin ...");
    let pool = ThreadPool::new(4);
    for stream in listener.incoming() {
        let stream = stream.unwrap();
        pool.excute(|| {
          handle_connection(stream);
        });
    }
}

fn handle_connection(mut stream: TcpStream) {
  let mut buffer = [0; 1024];
  stream.read(&mut buffer).unwrap();

  let get = b"GET / HTTP/1.1\r\n";
  let sleep = b"GET /sleep HTTP/1.1\r\n";

  let (status_line, filename) = if buffer.starts_with(get) {
      ("HTTP/1.1 200 OK\r\n\r\n", "index.html")
  } else if buffer.starts_with(sleep) {
      thread::sleep(Duration::from_secs(10));
      ("HTTP/1.1 200 OK\r\n\r\n", "index.html")
  } else {
      ("HTTP/1.1 404 NOT FOUND\r\n\r\n", "404.html")
  };
  let filename=format!("resource/{}",filename);
  let contents = fs::read_to_string(filename).unwrap();

  let response = format!("{}{}", status_line, contents);

  stream.write(response.as_bytes()).unwrap();
  stream.flush().unwrap();
}
