### Remote debugging  
Run `run_remote_debug.sh <filename>` and `debug.sh <filename>` in separate terminals. While in gdb, type `target remote localhost:1234` to remote debug.  
If port is already in use:  
`netstat -tulpn | grep <port number>`  
`kill -9 <pid>`  

