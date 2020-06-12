/*************************************************************
 Date       : 2020年 06月 12日 星期五 21:27:01 CST
 Author     : rogerg6
 File       : serv.c
 Description:
*************************************************************/

#include <stdio.h>
#include <stdlib.h>

main()
{
	int sock, fd;
	sock = make_server_socket(port);
	if (sock == -1)
		exit(1);

	while (1) {
		fd = accept(sock, NULL, NULL);		// take next call
		if (fd == -1)
			break;
		process_request(fd);				// chat with client
		close(fd);
	}
}