#include "calc.h"
#include <stdlib.h>  

int *
dot_product_1_svc(vectors *argp, struct svc_req *rqstp)
{
	static int result;
	result = 0;

	for (int i = 0; i < argp->n; i++) {
		result += argp->x.x_val[i] * argp->y.y_val[i];
	}

	return &result;
}

average_result *
average_vectors_1_svc(vectors *argp, struct svc_req *rqstp)
{
	static average_result result;
	double sum_x = 0, sum_y = 0;

	for (int i = 0; i < argp->n; i++) {
		sum_x += argp->x.x_val[i];
		sum_y += argp->y.y_val[i];
	}

	result.avg_x = sum_x / argp->n;
	result.avg_y = sum_y / argp->n;

	return &result;
}

scale_result *
scale_vector_1_svc(scale_input *argp, struct svc_req *rqstp)
{
	static scale_result result;


	if (result.values.values_val != NULL) {
		free(result.values.values_val);
		result.values.values_val = NULL;
		result.values.values_len = 0;
	}

	result.values.values_len = argp->n;
	result.values.values_val = malloc(argp->n * sizeof(double));

	for (int i = 0; i < argp->n; i++) {
		result.values.values_val[i] = argp->r * argp->x.x_val[i];
	}

	return &result;
}

