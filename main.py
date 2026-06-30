from shard_renderer import Engine, Camera, PlayerController, Mat4

cam = Camera()
controller = PlayerController()

engine = Engine()
engine.initialize(1920, 1080, "Shard Renderer")

engine.hide_mouse()

test_tri = engine.create_mesh()

while not engine.should_close():
    dt = 0.016
    player_input = engine.get_input()
    controller.update(cam, player_input, dt)

    engine.update_mat4("uModel", Mat4.identity())
    engine.update_mat4("uView", cam.get_view())
    engine.update_mat4("uProj", cam.get_projection())

    engine.begin_frame()

    engine.draw_mesh(test_tri)

    engine.end_frame()

engine.shutdown()