# psqlsrv

C++20 project with access to PostgreSQL and HTTP.

Compile server and run:
```bash
make
./bin/server
```

Build server and run:
```bash
make build
server
```

Install server in ~/bin and run:
```bash
make install
server
```

Build and install server in ~/bin and run:
```bash
make all
server
```

Run server in ~/bin:
```bash
make run
```

Debug server in ~/bin:
```bash
make rundebug
```

Clean intermediate *.o files:
```bash
make cleanobj
```

Clean intermediate files and server:
```bash
make clean
```

Clean all intermediate files and installed server:
```bash
make cleanall
```

Create a simple backup of source files in attic:
```bash
make backup
```

Clean simple backup in attic:
```bash
make cleanbackup
```

Download latest version of cpp-httplib and compare:
```bash
make deps
```

