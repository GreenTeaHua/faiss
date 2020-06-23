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
#include <faiss/IndexIVFFlat.h>


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
        for(int j = 0; j < d; j++)
            //xb[d * i + j] = drand48();
			xb[d * i + j] = dis(gen);
        xb[d * i] += i / 1000.;
    }

    for(int i = 0; i < nq; i++) {
        for(int j = 0; j < d; j++)
            //xq[d * i + j] = drand48();
			xq[d * i + j] = dis(gen);
        xq[d * i] += i / 1000.;
    }


    int nlist = 100;
    int k = 4;

    faiss::IndexFlatL2 quantizer(d);       // the other index
    faiss::IndexIVFFlat index(&quantizer, d, nlist, faiss::METRIC_L2);
    // here we specify METRIC_L2, by default it performs inner-product search
    assert(!index.is_trained);
    index.train(nb, xb.data());
    assert(index.is_trained);
    index.add(nb, xb.data());

    {       // search xq
		std::vector<int64_t> I(k * nq);
		std::vector<float> D(k * nq);

        index.search(nq, xq.data(), k, D.data(), I.data());

        printf("I=\n");
        for(int i = nq - 5; i < nq; i++) {
            for(int j = 0; j < k; j++)
                printf("%5ld ", I[i * k + j]);
            printf("\n");
        }

        index.nprobe = 10;
        index.search(nq, xq.data(), k, D.data(), I.data());

        printf("I=\n");
        for(int i = nq - 5; i < nq; i++) {
            for(int j = 0; j < k; j++)
                printf("%5ld ", I[i * k + j]);
            printf("\n");
        }
    }
    return 0;
}
