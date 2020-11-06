#include "OBJLoader.h"
#include "Matrix3D.h"

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <vector>
#include <float.h>
#include <math.h>
#include <string>

#define WIDTH 1024
#define HEIGHT 768

uint32_t *pixels;
float *depthBuffer;
Matrix3D model;
Matrix3D view;
Matrix3D projection;
Matrix3D mvp;
std::string rootpath;

struct Vertex {
    Vector3D position;
    Vector3D normal;
    Vector3D color;
    Vector3D uv;
};

void buildProjection(float near, float far, float fieldOfView, float width, float height, Matrix3D &out) {
    const float PI = 3.141592653589793f;
    float aspect = width / height;
    float zoom = 1 / tanf(PI * fieldOfView / 360.0f);
    float w = zoom / aspect;
    out.identity();
    out.rawData[0] = w;
    out.rawData[5] = zoom;
    out.rawData[10] = far / (far - near);
    out.rawData[11] = 1.0f;
    out.rawData[14] = -near * out.rawData[10];
    out.rawData[1] = out.rawData[2] = out.rawData[3] = out.rawData[4] =
    out.rawData[6] = out.rawData[7] = out.rawData[8] = out.rawData[9] =
    out.rawData[12]= out.rawData[13]= out.rawData[15]= 0.0f;
}

void initSDL() {
    depthBuffer = new float[WIDTH * HEIGHT];
    pixels = new uint32_t[WIDTH * HEIGHT];
}

std::vector<Vertex> vertexs;
std::vector<unsigned int> indices;

void loadMesh() {
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
    LoadOBJ((rootpath + "/assets/suzanne.obj").c_str(), vertices, uvs, normals, indices);
    int length = (int)vertices.size() / 3;
    for (int i = 0; i < length; i++) {
        Vertex vert;
        vert.position.setTo(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2]);
        vert.normal.setTo(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
        vert.uv.setTo(uvs[i * 2 + 0], uvs[i * 2 + 1], 0, 1.0f);
        vert.color.copyFrom(vert.normal);
        vert.color.normalize();
        vertexs.push_back(vert);
    }
}

Vertex projectAnd2Screen(const Vertex &v) {
    Vertex ret;
    mvp.transformVector(v.position, ret.position);
    ret.position.x /= ret.position.w;
    ret.position.y /= ret.position.w;
    ret.position.z /= ret.position.w;
    ret.position.x =  ret.position.x * WIDTH + WIDTH / 2;
    ret.position.y = -ret.position.y * HEIGHT + HEIGHT / 2;
    ret.color  = v.color;
    ret.normal = v.normal;
    ret.uv     = v.uv;
    return ret;
}

Vector3D lerp(const Vector3D &a, const Vector3D &b, float factor) {
    Vector3D ret;
    ret.x = a.x + (b.x - a.x) * factor;
    ret.y = a.y + (b.y - a.y) * factor;
    ret.z = a.z + (b.z - a.z) * factor;
    return ret;
}

Vertex lerp(const Vertex &a, const Vertex &b, float factor) {
    Vertex ret;
    ret.position = lerp(a.position, b.position, factor);
    ret.color = lerp(a.color, b.color, factor);
    ret.uv = lerp(a.uv, b.uv, factor);
    ret.normal = a.normal;
    return ret;
}

uint32_t rgba2color(float r, float g, float b, float a) {
    uint32_t R = (uint32_t)(r * 255.0f);
    uint32_t G = (uint32_t)(g * 255.0f);
    uint32_t B = (uint32_t)(b * 255.0f);
    uint32_t A = (uint32_t)(a * 255.0f);
    uint32_t value = (B << 24) | (G << 16) | (R << 8) | A;
    return value;
}

void setPixels(int x, int y, float z, uint32_t color) {
    int index = (int)(WIDTH * y + x);
    float depth = depthBuffer[index];
    if (depth < z) {
        return;
    }
    depthBuffer[index] = z;
    pixels[index] = color;
}

void drawPoint(const Vertex &v) {
    if (v.position.x >= 0 && v.position.y >= 0 && v.position.x <= WIDTH && v.position.y <= HEIGHT) {
        setPixels(v.position.x, v.position.y, v.position.z, rgba2color(v.color.x, v.color.y, v.color.z, 1.0f));
    }
}

void drawLine(const Vertex &v1, const Vertex &v2, const int &y) {
    int l = v1.position.x;
    int r = v2.position.x;
    int invert = r > l ? 1 : -1;
    float factor = 0;
    for (int x = l, i = 0; i < (r - l) * invert + 1; i++, x += invert) {
        if (r != l) {
            factor = (float)(x - l) / (r - l);
        }
        Vertex v = lerp(v1, v2, factor);
        drawPoint(v);
    }
}

void drawTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2) {
    const Vertex *a = &v0;
    const Vertex *b = &v1;
    const Vertex *c = &v2;
    if (a->position.y > b->position.y) {
        std::swap(a, b);
    }
    if (b->position.y > c->position.y) {
        std::swap(b, c);
    }
    if (a->position.y > b->position.y) {
        std::swap(a, b);
    }
    
    float mf = (float)(b->position.y - a->position.y) / (c->position.y - a->position.y);
    Vertex middle = lerp(*a, *c, mf);
    
    int startY = a->position.y;
    int endY = b->position.y;
    
    for (int y = startY; y <= endY; y++) {
        float factor = 0;
        if (endY != startY) {
            factor = (float)(y - startY) / (endY - startY);
        }
        Vertex va = lerp(*a, *b, factor);
        Vertex vb = lerp(*a, middle, factor);
        drawLine(va, vb, y);
    }
    
    startY = b->position.y;
    endY = c->position.y;
    for (int y = startY; y <= endY; y++) {
        float factor = 0;
        if (endY != startY) {
            factor = (float)(y - startY) / (endY - startY);
        }
        Vertex va = lerp(*b, *c, factor);
        Vertex vb = lerp(middle, *c, factor);
        drawLine(va, vb, y);
    }
}

void drawMesh() {
    
    for (unsigned int i = 0; i < indices.size(); i += 3) {
        int idx0 = indices[i];
        int idx1 = indices[i + 1];
        int idx2 = indices[i + 2];
        
        Vertex a = vertexs[indices[i]];
        Vertex b = vertexs[indices[i+1]];
        Vertex c = vertexs[indices[i+2]];
        
        Vertex v0 = projectAnd2Screen(a);
        Vertex v1 = projectAnd2Screen(b);
        Vertex v2 = projectAnd2Screen(c);
        
        drawTriangle(v0, v1, v2);
    }
    
}

void initCamera() {
    view.setPosition(0, 0, -10);
    view.invert();
    buildProjection(0.1f, 1.0f, 75.0f, 1024.0f, 768.0f, projection);
}

void updateMVP(float rotation) {
    model.appendRotation(rotation, Vector3D::Y_AXIS);
    mvp.identity();
    mvp.append(model);
    mvp.append(view);
    mvp.append(projection);
}

void clear() {
    memset(pixels, 0, sizeof(uint32_t) * WIDTH * HEIGHT);
    std::fill(depthBuffer, depthBuffer + WIDTH * HEIGHT, FLT_MAX);
}

void writeImage(const std::string& i)
{
	stbi_write_png((rootpath + "/out" + i + ".png").c_str(), WIDTH, HEIGHT, 4, pixels, WIDTH * 4);
}

int main(int argc, char *argv[]) {
	
	rootpath = argv[0];
	rootpath = rootpath.substr(0, rootpath.find_last_of("/\\"));
	for (int i = 0; i < rootpath.size(); ++i)
	{
		if (rootpath[i] == '\\')
		{
			rootpath[i] = '/';
		}
	}

    initSDL();
    initCamera();
    loadMesh();

	for (int i = 0; i < 12; ++i)
	{
		clear();
		updateMVP(i * 30);
		drawMesh();
		writeImage(std::to_string(i));
	}

    return 0;
}
