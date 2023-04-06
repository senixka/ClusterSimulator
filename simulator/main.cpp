#include "Cluster.h"

#include <cstdlib>
#include <random>


using namespace std;

int main() {
    std::srand(179);

    char env[] = "PYTHONMALLOC=malloc";
    putenv(env);

    Cluster cluster;
    cluster.Run();

    return 0;
}
