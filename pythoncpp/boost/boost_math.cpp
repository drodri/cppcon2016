#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <string>
#include <numeric>
#include <vector>
#include <set>

#include "converter.h"

int add(int i, int j) {
    return i + j;
}

struct Food {
    float quantity;
};
struct Water {
    float amount;
};
struct Looney {
    Looney(const std::string &name="Silvester") : name(name), happiness(0.0f) { }
    void setName(const std::string &name_="Tweety") { name = name_; }
    const std::string &getName() const { return name; }

    void give(const Food& food) { happiness += food.quantity;}
    void give(const Water& water) { happiness += water.amount;}

    bool operator == (const Looney& p){
        return p.name == name;
    }
    std::string name;
    float happiness;
    std::vector<std::string> friends;
};

namespace py = boost::python;

/*template <class T>
py::list toPythonList(std::vector<T> vector) {
    py::list list;
    for (auto iter = vector.begin(); iter != vector.end(); ++iter) {
        list.append(*iter);
    }
    return list;
}
template <class T>
std::vector<T> toCppVector(const py::list& list) {
    std::vector<T> vector;
    for (int i = 0; i < len(list); ++i){
        auto Looney = py::extract<Looney>(list[i]);
        vector.push_back(Looney);
    }
    return vector;
}
template< typename T >
inline
std::vector< T > to_std_vector(const py::list& list){
    return std::vector< T >( py::stl_input_iterator< T >( list ),
                             py::stl_input_iterator< T >( ) );
}*/

float average(const std::vector<Looney>& v){
    return std::accumulate(std::begin(v), std::end(v), 0.0f,
                          [](float a, Looney b) { return a + b.happiness;}) / v.size();
}

std::set<std::string> collect(const std::vector<Looney>& v){
    std::set<std::string> result;
    for(Looney p: v){
        result.insert(std::begin(p.friends), std::end(p.friends));
    }
    
    return result;
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(setname_overloads, Looney::setName, 0, 1)

struct vector_to_list{
    static PyObject* convert(const std::set<std::string>& v){
        PyObject* result = PySet_New(NULL);
        for(const std::string& s: v){
            PySet_Add(result, Py_BuildValue("s", s.c_str()));     
        }
        return result;
      }
};

BOOST_PYTHON_MODULE(boost_math){

    //py::class_<std::set<std::string>>("set_strings")
    //    .def(py::vector_indexing_suite<std::set<std::string>>() );
    py::to_python_converter<std::set<std::string>, vector_to_list>();

    // Register interable conversions.
    iterable_converter()
        // Build-in type.
        //.from_python<std::vector<double> >()
        // Each dimension needs to be convertable.
        .from_python<std::vector<std::string> >()
        //.from_python<std::set<std::string> >()
        //.from_python<std::vector<std::vector<std::string> > >()
        // User type.
        .from_python<std::vector<Looney> >()
        ;

    py::def("add", add);
    py::def("average", average);
    py::def("collect", collect);

    py::class_<std::vector<Looney> >("stl_vector_Looney")
        .def(py::vector_indexing_suite<std::vector<Looney> >())
    ;

    py::class_<Food>("Food")
        .def_readwrite("quantity", &Food::quantity);
    py::class_<Water>("Water")
        .def_readwrite("amount", &Water::amount);
    
    py::class_<Looney>("Looney", py::init<py::optional<std::string>>())
        .def_readwrite("happiness", &Looney::happiness)
        .def_readwrite("friends", &Looney::friends)
        .def("setName", &Looney::setName,
                setname_overloads())
        .def("getName", &Looney::getName,
                py::return_value_policy<py::copy_const_reference>())
        .def("give", (void (Looney::*)(const Food &)) &Looney::give)
        .def("give", (void (Looney::*)(const Water &)) &Looney::give);;
}