#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <string>
#include <numeric>
#include <vector>
#include <set>

#include "converter.h"

PyObject * log_function;

void call_log(const std::string& msg) {
    PyObject* value = Py_BuildValue("(s)", msg.c_str());
    
    PyObject* result = PyObject_CallObject(log_function, value);

    Py_XDECREF(value);
    Py_XDECREF(result); 
}

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

    void give( Food& food) { happiness += food.quantity; food.quantity=0;}
    void give(const Water& water) { happiness += water.amount;}

    std::string name;
    float happiness;
    std::vector<std::string> friends;
};

void set_log_function(PyObject *f){
    Py_XDECREF(log_function);
    Py_INCREF(f);
    log_function=f;
}

namespace py = boost::python;

py::object multiply(py::object a, py::object b){
    py::object c = a * b;
    return c;
}

float average(const std::vector<Looney>& v){
    call_log("Computing average");
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

struct set_to_set{
    static PyObject* convert(const std::set<std::string>& v){
        PyObject* result = PySet_New(NULL);
        for(const std::string& s: v){
            PySet_Add(result, Py_BuildValue("s", s.c_str()));     
        }
        Py_INCREF(result);
        return result;
      }
};
struct vector_to_list{
    static PyObject* convert(const std::vector<std::string>& v){
        PyObject* result = PyList_New(NULL);
        for(const std::string& s: v){
            PySet_Add(result, Py_BuildValue("s", s.c_str()));     
        }
        Py_INCREF(result);
        return result;
      }
};

BOOST_PYTHON_MODULE(boost_math){

    py::class_<std::vector<std::string>>("vector_strings")
        .def(py::vector_indexing_suite<std::vector<std::string>>() );
    py::to_python_converter<std::set<std::string>, set_to_set>();
    //py::to_python_converter<std::vector<std::string>, vector_to_list>();

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
    py::def("multiply", multiply);
    py::def("set_log_function", &set_log_function);

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
        .def("give", (void (Looney::*)( Food &)) &Looney::give)
        .def("give", (void (Looney::*)(const Water &)) &Looney::give);;
}