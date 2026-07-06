from shard_renderer import Engine, Camera, PlayerController, Mat4, Vec3
from obj_parser import load_models
from texture_loader import load_texture, load_env_map
import time

cam = Camera()
controller = PlayerController()

engine = Engine()
engine.initialize(1920, 1080, "Shard Renderer")

engine.hide_mouse()

vertices, indices = load_models(["assets/models/Warehouse.obj"])

mesh = engine.create_mesh(vertices, indices)
mat = engine.create_material("assets/shaders/standard.frag", "assets/shaders/standard.vert")

albedo, _ = load_texture(engine, "assets/textures/Empty.png", "assets/textures/Empty.png")
engine.bind_texture(albedo, 0)
engine.update_int(mat, "uAlbedo", 0)

normal, _ = load_texture(engine, "assets/textures/EmptyNormal.png", "assets/textures/EmptyNormal.png")
engine.bind_texture(normal, 1)
engine.update_int(mat, "uNormal", 1)

height, _ = load_texture(engine, "assets/textures/EmptyHeightmap.png", "assets/textures/EmptyHeightmap.png")
engine.bind_texture(height, 2)
engine.update_int(mat, "uHeightMap", 2)
engine.update_float(mat, "uHeightScale", 0.01)

orm, _ = load_texture(engine, "assets/textures/EmptyORM.png", "assets/textures/EmptyORM.png")
engine.bind_texture(orm, 3)
engine.update_int(mat, "uOrmMap", 3)

env, _, _, _, _ = load_env_map(engine, "assets/textures/Day-HDRI.exr")
engine.bind_texture(env, 4)
engine.update_int(mat, "uEnvMap", 4)

engine.update_float(mat, "uUVScale", 1.0)

light_dir = Vec3(-0.3, -1.0, -0.2)
engine.update_vec3(mat, "uLightDir", light_dir)

engine.update_float(mat, "uTonemapExposure", 3.0)

dt = 0
while not engine.should_close():
    s = time.perf_counter()
    player_input = engine.get_input()
    controller.update(cam, player_input, dt)

    engine.bind_texture(engine.get_shadow_depth(), 5)
    engine.update_int(mat, "uShadowMap", 5)

    engine.update_mat4(mat, "uView", cam.get_view())
    engine.update_mat4(mat, "uProj", cam.get_projection())

    engine.update_mat4(mat, "uLightSpace", engine.get_light_space())

    engine.update_vec3(mat, "uCamPos", cam.position)

    engine.begin_shadows(Vec3(-light_dir.x, -light_dir.y, -light_dir.z), cam.position)
    engine.draw_shadow(mesh, Mat4.identity())
    engine.end_shadows()

    engine.begin_frame()
    engine.draw_mesh(mesh, mat, Mat4.identity())
    engine.end_frame()

    dt = time.perf_counter() - s
engine.shutdown()