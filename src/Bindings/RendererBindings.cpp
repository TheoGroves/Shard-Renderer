#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "Math/Vec3.h"
#include "Math/Mat4.h"
#include "Core/Engine.h"
#include "Core/Camera.h"
#include "Math/Vec3.h"
#include "Gameplay/PlayerController.h"

namespace py = pybind11;

PYBIND11_MODULE(shard_render_engine, m)
{
    py::class_<Vec3>(m, "Vec3")
        .def(py::init<float, float, float>())
        .def_readwrite("x", &Vec3::x)
        .def_readwrite("y", &Vec3::y)
        .def_readwrite("z", &Vec3::z);

    py::class_<Mat4>(m, "Mat4")
        .def(py::init([] {
            return Mat4::Identity();
        }))
        .def_readwrite("m", &Mat4::m)
        .def_static("identity", &Mat4::Identity);

    m.def("translate", &Translate);
    m.def("perspective", &Perspective);
    m.def("look_at", &LookAt);

    py::class_<Engine>(m, "Engine")
        .def(py::init<>())
        .def("initialize", &Engine::Initialize)

        .def("create_mesh", 
            [](Engine& engine,
                py::array_t<float, py::array::c_style | py::array::forcecast> vertices,
                py::array_t<uint32_t, py::array::c_style | py::array::forcecast> indices)
            {
                auto v = vertices.request();
                auto i = indices.request();

                return engine.CreateMesh(
                    static_cast<float*>(v.ptr),
                    static_cast<size_t>(v.size),
                    static_cast<uint32_t*>(i.ptr),
                    static_cast<size_t>(i.size)
                );
            })


        .def("update_mat4", &Engine::UpdateMat4)
        .def("get_input", &Engine::GetInput)
        .def("should_close", &Engine::ShouldClose)
        .def("begin_frame", &Engine::BeginFrame)
        .def("draw_mesh", &Engine::DrawMesh)
        .def("hide_mouse", &Engine::HideMouse)
        .def("show_mouse", &Engine::ShowMouse)
        .def("end_frame", &Engine::EndFrame)
        .def("shutdown", &Engine::Shutdown);

    py::class_<Camera>(m, "Camera")
        .def(py::init<>())

        .def_readwrite("position", &Camera::position)
        .def_readwrite("forward", &Camera::forward)
        .def_readwrite("up", &Camera::up)
        .def_readwrite("right", &Camera::right)
        .def_readwrite("world_up", &Camera::worldUp)
        .def_readwrite("rotation", &Camera::rotation)

        .def_readwrite("fov", &Camera::fov)
        .def_readwrite("aspect", &Camera::aspect)
        .def_readwrite("near_plane", &Camera::nearPlane)
        .def_readwrite("far_plane", &Camera::farPlane)

        .def("get_view", &Camera::GetView)
        .def("get_projection", &Camera::GetProjection);
        
    m.def("forward_from_euler", &ForwardFromEuler);
    m.def("update_camera_vectors", &UpdateCameraVectors);

    py::class_<Input>(m, "Input")
        .def(py::init<>())
        .def_readwrite("forward", &Input::forward)
        .def_readwrite("backward", &Input::backward)
        .def_readwrite("left", &Input::left)
        .def_readwrite("right", &Input::right)
        .def_readwrite("mouse_dx", &Input::mouseDeltaX)
        .def_readwrite("mouse_dy", &Input::mouseDeltaY);

    py::class_<PlayerController>(m, "PlayerController")
        .def(py::init<>())
        .def_readwrite("move_speed", &PlayerController::moveSpeed)
        .def_readwrite("mouse_sensitivity", &PlayerController::mouseSensitivity)
        .def("update", &PlayerController::Update);


}