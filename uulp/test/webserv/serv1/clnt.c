/*************************************************************
 Date       : 2020年 06月 12日 星期五 21:52:22 CST
 Author     : rogerg6
 File       : clnt.c
 Description:
*************************************************************/

#include <stdio.h>
#include <stdlib.h>


main()
{
	int fd;

	fd = connect_to_server(host, port);
	if (fd == -1)
		exit(1);

	talk_with_server(fd);		// chat with server
	close(fd);
}