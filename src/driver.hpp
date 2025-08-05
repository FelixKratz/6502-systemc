#include <systemc>

template <class Object>
using DriverHandlerFunction = void(Object::*)();

template <class Object>
class SimulationDriver {
  public:
  Object* object;
  DriverHandlerFunction<Object> handler;
  virtual void wait(double delta) = 0;
  virtual void step(uint64_t count) = 0;

  SimulationDriver(Object* object, DriverHandlerFunction<Object> handler, std::string name) : handler(handler), object(object) {}
  virtual ~SimulationDriver() = default;
};

template <class Object>
class SystemCSimulationDriver : public SimulationDriver<Object>, public sc_core::sc_module {
  private:

  void systemc_thread() {
    (SimulationDriver<Object>::object->*SimulationDriver<Object>::handler)();
  }

  virtual void wait(double delta) override {
    sc_core::wait(sc_core::sc_time(delta, sc_core::SC_US));
  }

  public:
  SystemCSimulationDriver(Object* object, DriverHandlerFunction<Object> handler, std::string name) : sc_module(sc_core::sc_module_name(name.c_str())), SimulationDriver<Object>(object, handler, name) {
     SC_THREAD(systemc_thread);
   }

  virtual void step(uint64_t count) override {
    sc_start(static_cast<double>(count), sc_core::SC_US);
  }
};

template <class Object>
class ManualSimulationDriver : public SimulationDriver<Object> {
  virtual void wait(double delta) override {};
  virtual void step(double delta) override {};
};
