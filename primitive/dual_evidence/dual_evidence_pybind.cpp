/* dual_evidence_pybind.cpp — B4 pybind11 module dual_evidence_cpp
 * THE BEASTIE BOYZ / Blam · EXTERNAL-clean
 */
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "dual_evidence.hpp"
#include "spike_agent_gate.hpp"
#include "residual_full_path.hpp"

namespace py = pybind11;

static float sigma_from_numpy(py::array_t<float, py::array::c_style | py::array::forcecast> amps) {
    auto buf = amps.request();
    if (buf.ndim != 1) throw std::runtime_error("amps must be 1-D");
    return dual_evidence::spike_agent::sigma_from_amps(
        static_cast<float*>(buf.ptr), static_cast<int>(buf.shape[0]));
}

static py::dict compute_stream(py::array_t<float, py::array::c_style | py::array::forcecast> stream,
                               float threshold = 0.05f) {
    auto buf = stream.request();
    if (buf.ndim != 1) throw std::runtime_error("stream must be 1-D");
    auto s = dual_evidence::compute(static_cast<float*>(buf.ptr),
                                    static_cast<int>(buf.shape[0]), threshold);
    py::dict d;
    d["even_mean"] = s.even_mean;
    d["odd_mean"] = s.odd_mean;
    d["delta"] = s.delta;
    d["n_odd_events"] = s.n_odd_events;
    return d;
}

static py::dict residual_decide(py::array_t<float, py::array::c_style | py::array::forcecast> stream,
                                float classical_threshold = 0.08f,
                                float sigma_floor = 0.02f,
                                float expected_polarity = 0.f) {
    auto buf = stream.request();
    if (buf.ndim != 1) throw std::runtime_error("stream must be 1-D");
    auto d = dual_evidence::residual_full::decide(
        static_cast<float*>(buf.ptr), static_cast<int>(buf.shape[0]),
        classical_threshold, sigma_floor, expected_polarity);
    py::dict out;
    out["owned"] = d.owned;
    out["talent"] = static_cast<int>(d.talent);
    out["sigma_delta"] = d.sigma_delta;
    out["classical_score"] = d.classical_score;
    return out;
}

PYBIND11_MODULE(dual_evidence_cpp, m) {
    m.doc() = "Dual-evidence residual core (C++) — THE BEASTIE BOYZ / EXTERNAL-clean";

    m.def("sigma_amps", &sigma_from_numpy);
    m.def("compute", &compute_stream, py::arg("stream"), py::arg("threshold") = 0.05f);
    m.def("second_solid", [](float sigma, float expected_polarity, float floor) {
        return dual_evidence::second_solid(sigma, expected_polarity, floor);
    }, py::arg("sigma_delta"), py::arg("expected_polarity") = 0.f, py::arg("floor") = 0.005f);

    m.def("classical_choose", [](float L_rate, float L_cv, float R_rate, float R_cv) {
        dual_evidence::spike_agent::ChannelSense L, R;
        L.rate = L_rate; L.cv = L_cv; R.rate = R_rate; R.cv = R_cv;
        return static_cast<int>(dual_evidence::spike_agent::classical_choose(L, R));
    });
    m.def("dual_choose", [](float L_rate, float L_sigma, float R_rate, float R_sigma, float floor) {
        dual_evidence::spike_agent::ChannelSense L, R;
        L.rate = L_rate; L.sigma = L_sigma; R.rate = R_rate; R.sigma = R_sigma;
        return static_cast<int>(dual_evidence::spike_agent::dual_choose(L, R, floor));
    }, py::arg("L_rate"), py::arg("L_sigma"), py::arg("R_rate"), py::arg("R_sigma"),
       py::arg("floor") = 0.05f);

    m.def("residual_decide", &residual_decide,
          py::arg("stream"), py::arg("classical_threshold") = 0.08f,
          py::arg("sigma_floor") = 0.02f, py::arg("expected_polarity") = 0.f);

    py::enum_<dual_evidence::spike_agent::Action>(m, "Action")
        .value("STAY", dual_evidence::spike_agent::Action::STAY)
        .value("LEFT", dual_evidence::spike_agent::Action::LEFT)
        .value("RIGHT", dual_evidence::spike_agent::Action::RIGHT);

    py::enum_<dual_evidence::residual_full::ResidualTalent>(m, "ResidualTalent")
        .value("CHEAP", dual_evidence::residual_full::ResidualTalent::CHEAP)
        .value("FULL_GENERIC", dual_evidence::residual_full::ResidualTalent::FULL_GENERIC)
        .value("FULL_STRUCTURE", dual_evidence::residual_full::ResidualTalent::FULL_STRUCTURE)
        .value("FULL_HE", dual_evidence::residual_full::ResidualTalent::FULL_HE);
}
