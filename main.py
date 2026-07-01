from shard_renderer import Engine, Camera, PlayerController, Mat4
import time

cam = Camera()
controller = PlayerController()

engine = Engine()
engine.initialize(1920, 1080, "Shard Renderer")

engine.hide_mouse()

test_tri = engine.create_mesh()

dt = 0
while not engine.should_close():
    s = time.perf_counter()
    player_input = engine.get_input()
    controller.update(cam, player_input, dt)

    engine.update_mat4("uModel", Mat4.identity())
    engine.update_mat4("uView", cam.get_view())
    engine.update_mat4("uProj", cam.get_projection())

    engine.begin_frame()

    engine.draw_mesh(test_tri)

    engine.end_frame()
    dt = time.perf_counter() - s
engine.shutdown()