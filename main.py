from shard_renderer import Engine, Camera, PlayerController, Mat4
from obj_parser import load_models
from texture_loader import load_texture
import time

cam = Camera()
controller = PlayerController()

engine = Engine()
engine.initialize(1920, 1080, "Shard Renderer")

engine.hide_mouse()

vertices, indices = load_models(["assets/models/Suzanne.obj"])

engine.use_shader("assets/shaders/basic.frag", "assets/shaders/basic.vert")

albedo, _ = load_texture(engine, "assets/textures/empty.png", "assets/textures/empty.png")
engine.bind_texture(albedo, 0)
engine.update_int("uAlbedo", 0)

mesh = engine.create_mesh(vertices, indices)

dt = 0
while not engine.should_close():
    s = time.perf_counter()
    player_input = engine.get_input()
    controller.update(cam, player_input, dt)

    engine.update_mat4("uModel", Mat4.identity())
    engine.update_mat4("uView", cam.get_view())
    engine.update_mat4("uProj", cam.get_projection())

    engine.begin_frame()

    engine.draw_mesh(mesh, "assets/shaders/basic.frag", "assets/shaders/basic.vert")

    engine.end_frame()
    dt = time.perf_counter() - s
engine.shutdown()