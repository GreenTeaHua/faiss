/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <random>
#include <faiss/IndexFlat.h>
#include <faiss/gpu/GpuIndexFlat.h>
#include <faiss/gpu/GpuIndexIVFFlat.h>
#include <faiss/gpu/StandardGpuResources.h>


int main() {
    int d = 64;                            // dimension
    int nb = 100000;                       // database size
    int nq = 10000;                        // nb of queries

	std::vector<float> xb(d * nb);
	std::vector<float> xq(d* nq);

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<float> dis(0, 1);
    for(int i = 0; i < nb; i++) {
		for (int j = 0; j < d; j++)
			//xb[d * i + j] = drand48();
			xb[d * i + j] = dis(gen);
        xb[d * i] += i / 1000.;
    }

    for(int i = 0; i < nq; i++) {
		for (int j = 0; j < d; j++)
			//xq[d * i + j] = drand48();
			xq[d * i + j] = dis(gen);
        xq[d * i] += i / 1000.;
    }

    faiss::gpu::StandardGpuResources res;

    // Using a flat index

    faiss::gpu::GpuIndexFlatL2 index_flat(&res, d);

    printf("is_trained = %s\n", index_flat.is_trained ? "true" : "false");
    index_flat.add(nb, xb.data());  // add vectors to the index
    printf("ntotal = %ld\n", index_flat.ntotal);

    int k = 4;

    {       // search xq
        std::vector<int64_t> I(k * nq);
        std::vector<float> D(k * nq);

        index_flat.search(nq, xq.data(), k, D.data(), I.data());

        // print results
        printf("I (5 first results)=\n");
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < k; j++)
                printf("%5ld ", I[i * k + j]);
            printf("\n");
        }

        printf("I (5 last results)=\n");
        for(int i = nq - 5; i < nq; i++) {
            for(int j = 0; j < k; j++)
                printf("%5ld ", I[i * k + j]);
            printf("\n");
        }
    }

    // Using an IVF index

    int nlist = 100;
    faiss::gpu::GpuIndexIVFFlat index_ivf(&res, d, nlist, faiss::METRIC_L2);
    // here we specify METRIC_L2, by default it performs inner-product search

    assert(!index_ivf.is_trained);
    index_ivf.train(nb, xb.data());
    assert(index_ivf.is_trained);
    index_ivf.add(nb, xb.data());  // add vectors to the index

    printf("is_trained = %s\n", index_ivf.is_trained ? "true" : "false");
    printf("ntotal = %ld\n", index_ivf.ntotal);

    {       // search xq
		std::vector<int64_t> I(k * nq);
		std::vector<float> D(k * nq);

        index_ivf.search(nq, xq.data(), k, D.data(), I.data());

        // print results
        printf("I (5 first results)=\n");
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < k; j++)
                printf("%5ld ", I[i * k + j]);
            printf("\n");
        }

        printf("I (5 last results)=\n");
        for(int i = nq - 5; i < nq; i++) {
            for(int j = 0; j < k; j++)
                printf("%5ld ", I[i * k + j]);
            printf("\n");
        }
    }

    return 0;
}
