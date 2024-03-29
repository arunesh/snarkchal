#include <cstdio>
#include <vector>

#include <libff/algebra/curves/mnt753/mnt4753/mnt4753_pp.hpp>

using namespace libff;

void write_mnt4_fq(FILE* output, Fq<mnt4753_pp> x) {
  fwrite((void *) x.mont_repr.data, libff::mnt4753_q_limbs * sizeof(mp_size_t), 1, output);
}

Fq<mnt4753_pp> read_mnt4_fq(FILE* input) {
  // bigint<mnt4753_q_limbs> n;
  Fq<mnt4753_pp> x;
  fread((void *) x.mont_repr.data, libff::mnt4753_q_limbs * sizeof(mp_size_t), 1, input);
  return x;
}

void print_fq(Fq<mnt4753_pp> x) {
    int size = libff::mnt4753_q_limbs * sizeof(mp_size_t);
    uint8_t* array = (uint8_t*) x.mont_repr.data;
    for (int i = 0; i < size; i ++) {
        printf("%02x", array[i]);
    }
    printf("\nsize = %d * %d = %d\n", libff::mnt4753_q_limbs, sizeof(mp_size_t), size);
}

void write_mnt4_fq2(FILE* output, Fqe<mnt4753_pp> x) {
  write_mnt4_fq(output, x.c0);
  write_mnt4_fq(output, x.c1);
  printf("\n Output:\n");
  print_fq(x.c0);
  print_fq(x.c1);
}

Fqe<mnt4753_pp> read_mnt4_fq2(FILE* input) {
  Fq<mnt4753_pp> c0 = read_mnt4_fq(input);
  Fq<mnt4753_pp> c1 = read_mnt4_fq(input);
  return Fqe<mnt4753_pp>(c0, c1);
}

// The actual code for doing Fq2 multiplication lives in libff/algebra/fields/fp2.tcc
int main(int argc, char *argv[])
{
    // argv should be
    // { "main", "compute", inputs, outputs }

    mnt4753_pp::init_public_params();

    auto inputs = fopen(argv[2], "r");
    auto outputs = fopen(argv[3], "w");

    while (true) {
      size_t n;
      size_t elts_read = fread((void *) &n, sizeof(size_t), 1, inputs);

      if (elts_read == 0) { break; }

      std::vector<Fqe<mnt4753_pp>> x;
      for (size_t i = 0; i < n; ++i) {
        x.emplace_back(read_mnt4_fq2(inputs));
      }

      printf("\n Input 0:\n");
      print_fq(x.front().c0);
      print_fq(x.front().c1);

      std::vector<Fqe<mnt4753_pp>> y;
      for (size_t i = 0; i < n; ++i) {
        y.emplace_back(read_mnt4_fq2(inputs));
      }

      printf("\n Input 1:\n");
      print_fq(y.front().c0);
      print_fq(y.front().c1);

      printf("\n special MUL :\n");
      print_fq(x.front().c0 * y.front().c0);

      for (size_t i = 0; i < n; ++i) {
        write_mnt4_fq2(outputs, x[i] * y[i]);
      }
    }
    fclose(outputs);

    return 0;
}
