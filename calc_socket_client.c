#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"

int main() {
    while (1) {
        int sock;
        struct sockaddr_in server_addr;

        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("socket");
            exit(1);
        }

        // Set up server address
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

        // Connect to server
        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            perror("connect");
            exit(1);
        }

        int operation, n;
        int *x = NULL, *y = NULL;
        double r;

        printf("Choose operation (1=dot product, 2=average, 3=scale): ");
        fflush(stdout);
        scanf("%d", &operation);

        printf("Enter vector size (n): ");
        fflush(stdout);
        scanf("%d", &n);

        x = malloc(n * sizeof(int));
        if (operation == 1 || operation == 2) y = malloc(n * sizeof(int));
        if (operation == 3) {
            printf("Enter scalar r: ");
            fflush(stdout);
            scanf("%lf", &r);
        }

        printf("Enter elements of vector X:\n");
        fflush(stdout);
        for (int i = 0; i < n; i++) {
            printf("X[%d] = ", i);
            fflush(stdout);
            scanf("%d", &x[i]);
        }

        if (y) {
            printf("Enter elements of vector Y:\n");
            fflush(stdout);
            for (int i = 0; i < n; i++) {
                printf("Y[%d] = ", i);
                fflush(stdout);
                scanf("%d", &y[i]);
            }
        }

        // Send data to server
        write(sock, &operation, sizeof(int));
        write(sock, &n, sizeof(int));
        if (operation == 3) write(sock, &r, sizeof(double));
        write(sock, x, n * sizeof(int));
        if (y) write(sock, y, n * sizeof(int));

        // Read result
        if (operation == 1) {
            int result;
            read(sock, &result, sizeof(int));
            printf("Dot product: %d\n", result);
        } else if (operation == 2) {
            double avg_x, avg_y;
            read(sock, &avg_x, sizeof(double));
            read(sock, &avg_y, sizeof(double));
            printf("Average X: %.2f, Average Y: %.2f\n", avg_x, avg_y);
        } else if (operation == 3) {
            u_int len;
            read(sock, &len, sizeof(u_int));
            double *res = malloc(len * sizeof(double));
            read(sock, res, len * sizeof(double));
            printf("r * X = ");
            for (int i = 0; i < len; i++) printf("%.2f ", res[i]);
            printf("\n");
            free(res);
        }

        free(x);
        if (y) free(y);
        close(sock);


        char again;
        printf("Do you want to continue? (y/n): ");
        fflush(stdout);
        scanf(" %c", &again);
        if (again != 'y' && again != 'Y') break;
    }

    return 0;
}

