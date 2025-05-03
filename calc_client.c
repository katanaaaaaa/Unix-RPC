#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "calc.h" 
#include <arpa/inet.h>

#define PORT 12345

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "???s?: %s <rpc_server_ip>\n", argv[0]);
        exit(1);
    }

    char *rpc_host = argv[1];

    int sockfd, clientfd;
    struct sockaddr_in serv_addr;
    socklen_t addr_len = sizeof(serv_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
	    perror("socket");
	    exit(1);
	}
	
	
	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	

	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
	
	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
	    perror("bind");
	    close(sockfd);
	    exit(1);
	}


    listen(sockfd, 5);
    printf("Socket server listening on port %d...\n", PORT);

    // === MAIN LOOP: HANDLE CLIENTS ===
    while (1) {
        clientfd = accept(sockfd, (struct sockaddr*)&serv_addr, &addr_len);
        if (clientfd < 0) {
            perror("accept");
            continue;
        }

        int operation, n;
        double r;

        read(clientfd, &operation, sizeof(int));
        read(clientfd, &n, sizeof(int));
        if (operation == 3) {
            read(clientfd, &r, sizeof(double));
        }

        int *x = malloc(n * sizeof(int));
        int *y = NULL;
        read(clientfd, x, n * sizeof(int));
        if (operation == 1 || operation == 2) {
            y = malloc(n * sizeof(int));
            read(clientfd, y, n * sizeof(int));
        }

        // === RPC CLIENT ===
        CLIENT *clnt = clnt_create(rpc_host, CALC_PROG, CALC_VERS, "udp");
        if (clnt == NULL) {
            clnt_pcreateerror(rpc_host);
            close(clientfd);
            free(x);
            if (y) free(y);
            continue;
        }

        if (operation == 1) {
            // Dot product
            vectors args;
            args.n = n;
            args.x.x_len = args.y.y_len = n;
            args.x.x_val = x;
            args.y.y_val = y;

            int *result = dot_product_1(&args, clnt);
            if (result == NULL) {
                clnt_perror(clnt, "dot_product_1");
                int err = -1;
                write(clientfd, &err, sizeof(int));
            } else {
                write(clientfd, result, sizeof(int));
            }
        } else if (operation == 2) {
            // Average
            vectors args;
            args.n = n;
            args.x.x_len = args.y.y_len = n;
            args.x.x_val = x;
            args.y.y_val = y;

            average_result *res = average_vectors_1(&args, clnt);
            if (res == NULL) {
                clnt_perror(clnt, "average_vectors_1");
                double err = -1.0;
                write(clientfd, &err, sizeof(double));
                write(clientfd, &err, sizeof(double));
            } else {
                write(clientfd, &res->avg_x, sizeof(double));
                write(clientfd, &res->avg_y, sizeof(double));
            }
        } else if (operation == 3) {
            // Scale
            scale_input args;
            args.r = r;
            args.n = n;
            args.x.x_len = n;
            args.x.x_val = x;

            scale_result *res = scale_vector_1(&args, clnt);
            if (res == NULL) {
                clnt_perror(clnt, "scale_vector_1");
                u_int len = 0;
                write(clientfd, &len, sizeof(u_int));
            } else {
                u_int len = res->values.values_len;
                write(clientfd, &len, sizeof(u_int));
                write(clientfd, res->values.values_val, len * sizeof(double));
            }
        }

        
        clnt_destroy(clnt);
        free(x);
        if (y) free(y);
        close(clientfd);
    }

    close(sockfd);
    return 0;
}
