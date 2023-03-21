#include <iostream>
#include "utils.hpp"
#include "rbtree.hpp"
#include "obtree.hpp"
#include "timer.hpp"

auto const TEST_TIMES = 100; 

template<typename _T>
void run(const std::string & fileName){
    auto data = utils::readFile<_T>(fileName);
    auto P = utils::buildHistogram(data);
    auto K = utils::generateTypes<_T>();
    utils::Frequency<_T> f = utils::findFrequencies(data, P);

    std::vector<double> Q(P.size() + 1, 0);

    RbTree<_T> rbtree {K};
    ObTree<_T> obtree {P, Q, data};
    std::cout << "Costo OBTREE: " << obtree.build() << '\n';
    std::cout << "=====Resumen=====\n"

        << "Tamaño del mensaje: "<<data.size()<< '\n'
        << "=================\n"
        << "-------RedBlack Tree-------\n"
        << "Elemento Mínimo: '"<< f.minElem<<"' ,frecuencia = "<<f.minFrequency << ", produndidad = "<< rbtree.depth(f.minElem)<<'\n'
        << "Elemento Máximo: '"<< f.maxElem<<"' ,frecuencia = "<<f.maxFrequency << ", produndidad = "<< rbtree.depth(f.maxElem)<<'\n'
        << "Tiempo de búsqueda elemento menor: "<< timer::measure(rbtree, f.minElem, TEST_TIMES)<<"ns"<<'\n'
        << "Tiempo de búsqueda elemento Mayor: "<< timer::measure(rbtree, f.maxElem, TEST_TIMES)<<"ns"<<'\n'
        << "-------Optimal Tree-------\n"
        << "Elemento Mínimo: '"<< f.minElem<<"' ,frecuencia = "<<f.minFrequency << ", produndidad = "<< obtree.depth(f.minElem)<<'\n'
        << "Elemento Máximo: '"<< f.maxElem<<"' ,frecuencia = "<<f.maxFrequency << ", produndidad = "<< obtree.depth(f.maxElem)<<'\n'
        << "Tiempo de búsqueda elemento menor: "<< timer::measure(obtree, f.minElem, TEST_TIMES)<<"ns"<<'\n'
        << "Tiempo de búsqueda elemento Mayor: "<< timer::measure(obtree, f.maxElem, TEST_TIMES)<<"ns"<<'\n'
        ;
}

int main(int argc, char * argv[]){
    if (argc < 3){
        std::cerr << "Uso: ." <<argv[0] << " [nombre_archivo] [byte|word]\n";
        return 1;
    }
    std::string fileName{argv[1]};
    std::string type{argv[2]};

    if (type == "byte"){
        run<unsigned char>(fileName);
    }else if(type == "word"){
        run<unsigned short>(fileName);
    }else{
        std::cerr << "Error: Tipo desconocido: [byte|word]\n";
        return 1;
    }
}