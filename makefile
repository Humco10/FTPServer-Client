all: ftp_client.exe

all: ftp_server.exe

ftp_client.exe: ftp_client.o
				gcc -o ftp_client.exe ftp_client.o

ftp_server.exe: ftp_server.o
				gcc -o ftp_server.exe ftp_server.o

ftp_client.o:   ./client/ftp_client.c
				gcc -c ./client/ftp_client.c

ftp_server.o:   ./server/ftp_server.c
				gcc -c ./server/ftp_server.c

clean:
	rm ftp_client.o ftp_client.exe
	rm ftp_server.o ftp_server.exe