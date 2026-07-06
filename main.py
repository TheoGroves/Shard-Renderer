from shard_renderer import Engine, Camera, PlayerController, Mat4, Vec3
from obj_parser import load_models
from materials import PBRMaterial, SkyboxMaterial
import time

cam = Camera()
controller = PlayerController()

engine = Engine()
engine.initialize(1920, 1080, "Shard Renderer")

engine.hide_mouse()

warehouse_vertices, warehouse_indices = load_models(["assets/models/Warehouse.obj"])
warehouse_mesh = engine.create_mesh(warehouse_vertices, warehouse_indices)
mat = PBRMaterial(engine)

skybox_vertices, skybox_indices = load_models(["assets/models/Cube.obj"])
skybox_mesh = engine.create_mesh(skybox_vertices, skybox_indices)
skybox = SkyboxMaterial(engine)

light_dir = Vec3(-0.3, -1.0, -0.2)

dt = 1/60
try:
    while not engine.should_close():
        s = time.perf_counter()
        player_input = engine.get_input()
        controller.update(cam, player_input, dt)

        mat.update(engine, cam, light_dir)
        skybox.update(engine, cam)

        engine.begin_shadows(Vec3(-light_dir.x, -light_dir.y, -light_dir.z), cam.position)
        engine.draw_shadow(warehouse_mesh, Mat4.identity())
        engine.end_shadows()

        engine.begin_frame()
        engine.disable_depth_test()
        engine.disable_cull_face()
        engine.draw_mesh(skybox_mesh, skybox.material, Mat4.identity())
        engine.enable_depth_test()
        engine.enable_cull_face()
        engine.draw_mesh(warehouse_mesh, mat.material, Mat4.identity())
        engine.end_frame()

        dt = time.perf_counter() - s
        time.sleep(max(0, 1/60-dt))
        dt = time.perf_counter() - s
finally:
    engine.shutdown()