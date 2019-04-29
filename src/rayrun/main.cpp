/*
# TODOs
- MSE�v�Z�������쐬����
- ���Ԍv���������쐬����
- �\�[�X�R�[�h�����J����
- 

# ���[��
- ���͂��ꂽ���b�V���f�[�^�ƃJ������񂩂�AO�̉摜���o�͂���
- �w��MSE�ȉ��̏ꍇ�͎��i
- GPU�֎~
- Embree�Ȃǂ̊O���̌�������G���W���͋֎~
- �S�Ẳ摜�����i���������ŁA�ł������I���������̂��D��

# �Q���҂ɍH�v���Ă��炦�����Ȃ��ƈꗗ
- ���񉻂���
- cos���ŏ����炩����
- QMC����
- AO�̕��U��������𔻒肷��
- shot��/image�̃T�C�Y�����������construction��}������
- shot��/image�̃T�C�Y���ُ�ɑ傫����΍H�v������
- �ُ�Ɂ������傫����΍H�v������
- raystream�ɂ���
*/

//
#include "rayrun.hpp"
//
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
//
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "picojson.h"
//
#include <vector>
#include <string>
#include <cstdint>

//
static std::tuple<std::vector<float>, std::vector<uint32_t>> loadMesh(const std::string& filename)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;
    std::vector<uint32_t> indices;
    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), nullptr, true);
    //
    for (auto& shape : shapes)
    {
        for (auto& index : shape.mesh.indices)
        {
            indices.push_back(index.vertex_index);
        }
    }
    return { attrib.vertices , indices };
}

static std::tuple<std::vector<float>, std::vector<uint32_t>> loadTriangle()
{
    std::vector<float> vs;
    vs.push_back(+0.0f); vs.push_back(+1.0f); vs.push_back(+0.0f);
    vs.push_back(-1.0f); vs.push_back(-1.0f); vs.push_back(+0.0f);
    vs.push_back(+1.0f); vs.push_back(-1.0f); vs.push_back(+0.0f);
    std::vector<uint32_t> is;
    is.push_back(0);
    is.push_back(1);
    is.push_back(2);
    return { vs, is };
}

//
static void testMain(RayRunFun rayRun)
{
    // obj�����[�h
    auto[vertices, indices] = loadMesh("../asset/moriknob.obj");
    // �����o����C���[�W�̏���
    const int32_t width = 1024;
    const int32_t height = 1024;
    //
    std::vector<Shot> shots;
    Shot shot;
    shot.pos[0] =   0.01f;
    shot.pos[1] =   7.0f;
    shot.pos[2] =   -7.0f;
    shot.dir[0] =   0.0f;
    shot.dir[1] =   -1.0f;
    shot.dir[2] =   1.0f;
    shot.up[0] = 0.0f;
    shot.up[1] = 1.0f;
    shot.up[2] = 0.0f;
    shot.fovy = 3.141592f / 6.0f;
    shot.width = width;
    shot.height = height;
    shot.image = (float*)malloc(shot.width*shot.height*sizeof(float)*3);
    shots.push_back(shot);

    // �e�X�g���Ăяo��
    rayRun(vertices.data(), vertices.size()/3, indices.data(), indices.size()/3, shots.data(), shots.size());

    // �C���[�W�����o��
    std::vector<uint8_t> aos;
    for (auto& shot : shots)
    {
        aos.clear();
        for (int32_t pi=0;pi<width*height*3;++pi)
        {
            const uint8_t ao = uint8_t(std::max(std::min(int32_t(shot.image[pi] * 254.0f + 0.5f),255),0));
            aos.push_back(ao);
        }
        stbi_flip_vertically_on_write(true);
        stbi_write_png("test.png", width, height, 3, aos.data(), sizeof(uint8_t)*width*3);
    }
    // TODO: diff�����s����
}

//
void main()
{
    HMODULE dll = LoadLibrary("refimp.dll");
    const RayRunFun rayRun = (RayRunFun)GetProcAddress(dll, "rayRun");
    testMain(rayRun);
    FreeLibrary(dll);
}

