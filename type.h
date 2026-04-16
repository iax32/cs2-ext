#pragma once
#include <algorithm> // std::clamp (C++17)
#include <array>
#include <cmath> // std::sqrt
#include <limits>
#include <ostream> // std::ostream
#include <imgui.h>



/*
____   ____             __
\   \ /   /____   _____/  |_  ___________  ______
 \   Y   // __ \_/ ___\   __\/  _ \_  __ \/  ___/
  \     /\  ___/\  \___|  | (  <_> )  | \/\___ \
   \___/  \___  >\___  >__|  \____/|__|  /____  >
              \/     \/                       \/
*/

struct Vector2 {
    float x{ }, y{ };

    constexpr Vector2() = default;
    constexpr Vector2(float source_x, float source_y) : x(source_x), y(source_y) {}

    // ----- compound assignment -----
    Vector2& operator+=(const Vector2& source) {
        x += source.x;
        y += source.y;
        return *this;
    }
    Vector2& operator-=(const Vector2& source) {
        x -= source.x;
        y -= source.y;
        return *this;
    }
    Vector2& operator*=(const Vector2& source) {
        x *= source.x;
        y *= source.y;
        return *this;
    } // element-wise
    Vector2& operator/=(const Vector2& source) {
        x /= source.x;
        y /= source.y;
        return *this;
    } // element-wise

    Vector2& operator*=(float s) {
        x *= s;
        y *= s;
        return *this;
    }
    Vector2& operator/=(float s) {
        x /= s;
        y /= s;
        return *this;
    }

    // ----- unary -----
    constexpr Vector2 operator-() const { return { -x, -y }; }

    // ----- non-compound -----
    friend Vector2 operator+(Vector2 left, const Vector2& source) { return left += source; }
    friend Vector2 operator-(Vector2 left, const Vector2& source) { return left -= source; }
    friend Vector2 operator*(Vector2 left, const Vector2& source) { return left *= source; } // element-wise
    friend Vector2 operator/(Vector2 left, const Vector2& source) { return left /= source; } // element-wise

    friend Vector2 operator*(Vector2 v, float s) { return v *= s; }
    friend Vector2 operator*(float s, Vector2 v) { return v *= s; } // symmetry
    friend Vector2 operator/(Vector2 v, float s) { return v /= s; }

    // ----- comparisons -----
    bool operator==(const Vector2& source) const {
        return x == source.x && y == source.y;
    }

    bool operator!=(const Vector2& source) const {
        return !(*this == source);
    }

    // ----- vector math -----
    float Dot(const Vector2& source) const {
        return x * source.x + y * source.y;
    }

    // in 2D, cross product can be represented as a scalar (signed area)
    float Cross(const Vector2& source) const {
        return x * source.y - y * source.x;
    }

    float LengthSquared() const {
        return this->Dot(*this);
    }

    float Length() const {
        return std::sqrt(LengthSquared());
    }

    Vector2 Normalized() const {
        float len = Length();
        if (len > 0.0f) {
            return *this / len;
        }
        return { 0.0f, 0.0f };
    }

    void Normalize() {
        float len = Length();
        if (len > 0.0f) {
            *this /= len;
        }
    }

    // ----- i/o -----
    friend std::ostream& operator<<(std::ostream& os, const Vector2& v) {
        return os << "(" << v.x << ", " << v.y << ")";
    }

    // ----- utilities -----
    float Distance(const Vector2& other) const {
        return (other - *this).Length();
    }

    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
        return a + (b - a) * t;
    }

    static Vector2 Clamp(const Vector2& v, const Vector2& min, const Vector2& max) {
        return {
            std::clamp(v.x, min.x, max.x),
            std::clamp(v.y, min.y, max.y) };
    }
};

struct Vector3 {
    float x{ }, y{ }, z{ };

    constexpr Vector3() = default;
    constexpr Vector3(float source_x, float source_y, float source_z) : x(source_x), y(source_y), z(source_z) {}

    // ----- compound assignment -----
    Vector3& operator+=(const Vector3& source) {
        x += source.x;
        y += source.y;
        z += source.z;
        return *this;
    }
    Vector3& operator-=(const Vector3& source) {
        x -= source.x;
        y -= source.y;
        z -= source.z;
        return *this;
    }
    Vector3& operator*=(const Vector3& source) {
        x *= source.x;
        y *= source.y;
        z *= source.z;
        return *this;
    } // element-wise
    Vector3& operator/=(const Vector3& source) {
        x /= source.x;
        y /= source.y;
        z /= source.z;
        return *this;
    } // element-wise

    Vector3& operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    Vector3& operator/=(float s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    // ----- unary -----
    constexpr Vector3 operator-() const { return { -x, -y, -z }; }

    // ----- non-compound -----
    friend Vector3 operator+(Vector3 left, const Vector3& source) { return left += source; }
    friend Vector3 operator-(Vector3 left, const Vector3& source) { return left -= source; }
    friend Vector3 operator*(Vector3 left, const Vector3& source) { return left *= source; } // element-wise
    friend Vector3 operator/(Vector3 left, const Vector3& source) { return left /= source; } // element-wise

    friend Vector3 operator*(Vector3 v, float s) { return v *= s; }
    friend Vector3 operator*(float s, Vector3 v) { return v *= s; } // symmetry
    friend Vector3 operator/(Vector3 v, float s) { return v /= s; }

    // ----- comparisons -----
    bool operator==(const Vector3& source) const {
        return x == source.x && y == source.y && z == source.z;
    }

    bool operator!=(const Vector3& source) const {
        return !(*this == source);
    }

    // ----- vector math -----
    float Dot(const Vector3& source) const {
        return x * source.x + y * source.y + z * source.z;
    }

    Vector3 Cross(const Vector3& source) const {
        return {
            y * source.z - z * source.y,
            z * source.x - x * source.z,
            x * source.y - y * source.x };
    }

    float LengthSquared() const {
        return this->Dot(*this);
    }

    float Length() const {
        return std::sqrt(LengthSquared());
    }

    Vector3 Normalized() const {
        float len = Length();
        if (len > 0.0f) {
            return *this / len;
        }
        return { 0.0f, 0.0f, 0.0f };
    }

    void Normalize() {
        float len = Length();
        if (len > 0.0f) {
            *this /= len;
        }
    }

    // ----- i/o -----
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }

    // ----- utilities -----
    float distance(const Vector3& other) const {
        return (other - *this).Length();
    }

    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
        return a + (b - a) * t;
    }

    static Vector3 Clamp(const Vector3& v, const Vector3& min, const Vector3& max) {
        return {
            std::clamp(v.x, min.x, max.x),
            std::clamp(v.y, min.y, max.y),
            std::clamp(v.z, min.z, max.z) };
    }
};

struct Vector4 {
    float x{ }, y{ }, z{ }, w{ };

    constexpr Vector4() = default;
    constexpr Vector4(float source_x, float source_y, float source_z, float source_w)
        : x(source_x), y(source_y), z(source_z), w(source_w) {
    }

    // ----- compound assignment -----
    Vector4& operator+=(const Vector4& source) {
        x += source.x;
        y += source.y;
        z += source.z;
        w += source.w;
        return *this;
    }
    Vector4& operator-=(const Vector4& source) {
        x -= source.x;
        y -= source.y;
        z -= source.z;
        w -= source.w;
        return *this;
    }
    Vector4& operator*=(const Vector4& source) {
        x *= source.x;
        y *= source.y;
        z *= source.z;
        w *= source.w;
        return *this;
    } // element-wise
    Vector4& operator/=(const Vector4& source) {
        x /= source.x;
        y /= source.y;
        z /= source.z;
        w /= source.w;
        return *this;
    } // element-wise

    Vector4& operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }
    Vector4& operator/=(float s) {
        x /= s;
        y /= s;
        z /= s;
        w /= s;
        return *this;
    }

    // ----- unary -----
    constexpr Vector4 operator-() const { return { -x, -y, -z, -w }; }

    // ----- non-compound -----
    friend Vector4 operator+(Vector4 left, const Vector4& source) { return left += source; }
    friend Vector4 operator-(Vector4 left, const Vector4& source) { return left -= source; }
    friend Vector4 operator*(Vector4 left, const Vector4& source) { return left *= source; } // element-wise
    friend Vector4 operator/(Vector4 left, const Vector4& source) { return left /= source; } // element-wise

    friend Vector4 operator*(Vector4 v, float s) { return v *= s; }
    friend Vector4 operator*(float s, Vector4 v) { return v *= s; } // symmetry
    friend Vector4 operator/(Vector4 v, float s) { return v /= s; }

    // ----- comparisons -----
    bool operator==(const Vector4& source) const {
        return x == source.x && y == source.y && z == source.z && w == source.w;
    }

    bool operator!=(const Vector4& source) const {
        return !(*this == source);
    }

    // ----- vector math -----
    float Dot(const Vector4& source) const {
        return x * source.x + y * source.y + z * source.z + w * source.w;
    }

    float LengthSquared() const {
        return this->Dot(*this);
    }

    float Length() const {
        return std::sqrt(LengthSquared());
    }

    Vector4 Normalized() const {
        float len = Length();
        if (len > 0.0f) {
            return *this / len;
        }
        return { 0.0f, 0.0f, 0.0f, 0.0f };
    }

    void Normalize() {
        float len = Length();
        if (len > 0.0f) {
            *this /= len;
        }
    }

    // ----- i/o -----
    friend std::ostream& operator<<(std::ostream& os, const Vector4& v) {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    }

    // ----- utilities -----
    float Distance(const Vector4& other) const {
        return (other - *this).Length();
    }

    static Vector4 Lerp(const Vector4& a, const Vector4& b, float t) {
        return a + (b - a) * t;
    }

    static Vector4 Clamp(const Vector4& v, const Vector4& min, const Vector4& max) {
        return {
            std::clamp(v.x, min.x, max.x),
            std::clamp(v.y, min.y, max.y),
            std::clamp(v.z, min.z, max.z),
            std::clamp(v.w, min.w, max.w) };
    }
};

/*
   _____          __         .__
  /     \ _____ _/  |________|__|__  ___
 /  \ /  \\__  \\   __\_  __ \  \  \/  /
/    Y    \/ __ \|  |  |  | \/  |>    <
\____|__  (____  /__|  |__|  |__/__/\_ \
        \/     \/                     \/
*/

// We do this in a struct so it is more convienient to read it in memory
struct Matrix {
    std::array<float, 16> mvpMatrix;
};

const inline bool WorldToScreen(
    const Vector3 vecOrigin,
    Vector2& vecScreen,
    std::array<float, 16> mvpMatrix) {

    Vector4 clipCoords = { };
    clipCoords.x = vecOrigin.x * mvpMatrix[0] + vecOrigin.y * mvpMatrix[1] + vecOrigin.z * mvpMatrix[2] + mvpMatrix[3];
    clipCoords.y = vecOrigin.x * mvpMatrix[4] + vecOrigin.y * mvpMatrix[5] + vecOrigin.z * mvpMatrix[6] + mvpMatrix[7];
    clipCoords.w = vecOrigin.x * mvpMatrix[12] + vecOrigin.y * mvpMatrix[13] + vecOrigin.z * mvpMatrix[14] + mvpMatrix[15];

    if (clipCoords.w < 0.00001f)
        return false;

    Vector3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;

    vecScreen.x = NDC.x;
    vecScreen.y = NDC.y;

    return !(NDC.x < -1.0f || NDC.x > 1.0f || NDC.y < -1.0f || NDC.y > 1.0f);
}

inline float ScreenDistance(const Vector2& a, const Vector2& b) {
    return hypotf(a.x - b.x, a.y - b.y);
}



inline int clampi(int v, int lo, int hi) {
    return (v < lo) ? lo : (v > hi) ? hi
        : v;
}


constexpr float RAD2DEG = 180.0f / 3.14159265358979323846f;


inline ImVec2 NDCToScreen(
    const Vector2& ndc,
    int screenWidth,
    int screenHeight) {
    //screenHeight -= 50;
    // Convert from NDC (-1..1) to screen pixels (0..width, 0..height)
    ImVec2 out;
    out.x = (ndc.x + 1.0f) * 0.5f * screenWidth;
    out.y = (1.0f - ndc.y) * 0.5f * screenHeight;
    return out;
}
