/// some minigrep
///
/// # Examples
///
/// minigrep to poem.txt
/// ```
///
/// ```
use std::{env, process};

use minigrep::{run, Config};
fn main() {
    let args: Vec<String> = env::args().collect();

    let config = Config::new(&args).unwrap_or_else(|err| {
        println!("error : {}", err);
        process::exit(1)
    });
    if let Err(err) = run(config) {
        println!("error : {}", err);
        process::exit(1);
    }
}
