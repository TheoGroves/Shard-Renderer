from texture_loader import load_env_map
from texture_slots import *

class SkyboxMaterial:
    def __init__(self, engine):
        self.material = engine.create_material("assets/shaders/skybox.frag", "assets/shaders/skybox.vert")

        self.env, _, _, _, _ = load_env_map(engine, "assets/textures/Day-HDRI.exr")
        engine.bind_texture(self.env, ENV_MAP)
        engine.update_int(self.material, "uEnvMap", ENV_MAP)

    def update(self, engine, cam):
        engine.update_mat4(self.material, "uView", cam.get_view())
        engine.update_mat4(self.material, "uProj", cam.get_projection())