# ECS160AI
ECS 160 Project AI Scripts

## Installing LUA for Linux
1. Download LUA 5.2.4 [here](https://www/lua.org/ftp/lua-5.2.4.tar.gz)
2. Unzip the file and cd into the directory
3. Then run: 
```bash
	make linux
``` 
* if you get "fatal error: readline/readline.h" then run
```bash
	sudo apt-get install libreadline-dev
```
4. Finally run:
```bash
	sudo make install
```
