#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <numeric>
#include <pybind11/functional.h>

std::function<void(std::string)> log_function;


void set_log_function(const std::function<void(std::string)>& f){
    log_function=f;
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

    void give(const Food& food) { happiness += food.quantity;}
    void give(const Water& water) { happiness += water.amount;}

    std::string name;
    float happiness;
    std::vector<std::string> friends;
};

namespace py = pybind11;


py::float_ multiply(py::object a, py::object b){
    double pa = *a.cast<double*>();
    double pb= *b.cast<double*>();
    py::float_ c =  pa * pb;
    return c;
}

float average(const std::vector<Looney>& v){
    log_function("Computing the average");
    py::gil_scoped_release release;
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



PYBIND11_PLUGIN(pybind11_math) {
    py::module m("pybind11_math");
    m.def("add", &add);
    m.def("set_log_function", &set_log_function);
    m.def("multiply", multiply);
    m.def("average", &average);
    m.def("collect", &collect);

    py::class_<Food>(m, "Food")
        .def(py::init<>())
        .def_readwrite("quantity", &Food::quantity);

    py::class_<Water>(m, "Water")
        .def(py::init<>())
        .def_readwrite("amount", &Water::amount);

    py::class_<Looney>(m, "Looney")
        .def(py::init<const std::string &>(),
             py::arg("name") = std::string("Silvester"))
        .def("setName", &Looney::setName,
             py::arg("name") = std::string("Tweety"))
        .def("getName", &Looney::getName)
        .def_readwrite("happiness", &Looney::happiness)
        .def_readwrite("friends", &Looney::friends)
        .def("give", (void (Looney::*)(const Food&)) &Looney::give)
        .def("give", (void (Looney::*)(const Water&)) &Looney::give);

    return m.ptr();
}