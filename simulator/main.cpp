#include "Cluster.h"

#include <cstdlib>


using namespace std;

int main() {
    char env[] = "PYTHONMALLOC=malloc";
    putenv(env);

    Cluster cluster;
    cluster.Run();

    return 0;
}
