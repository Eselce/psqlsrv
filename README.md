# psqlsrv

C++20 project with access to PostgreSQL and HTTP.

Build server and run:
```bash
make
./server
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

Create a simple backup of source files in attic;
```bash
make attic
```

Download latest version of cpp-httplib and compare:
```bash
make deps
```

