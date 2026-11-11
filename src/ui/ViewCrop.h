#pragma once
#include <algorithm>

// Visible sub-rectangle of the video frame, in normalized texture coords.
// The crop always keeps the video's aspect ratio, so one `zoom` value
// (1 = whole frame, 16 = 1/16 of each side) fully describes its size.
struct ViewCrop {
    static constexpr float kMinZoom = 1.0f;
    static constexpr float kMaxZoom = 16.0f;

    float cx = 0.5f;    // crop centre, UV [0..1]
    float cy = 0.5f;
    float zoom = 1.0f;

    float Half() const { return 0.5f / zoom; }   // half-size of the crop in UV
    bool  IsZoomed() const { return zoom > 1.0001f; }

    // Keep the crop inside the frame. When a scale change makes the crop
    // poke out of the frame, the centre is pushed back in. At zoom == 1
    // this forces the centre to exactly (0.5, 0.5).
    void Clamp() {
        zoom = std::clamp(zoom, kMinZoom, kMaxZoom);
        const float h = Half();
        cx = std::clamp(cx, h, 1.0f - h);
        cy = std::clamp(cy, h, 1.0f - h);
    }

    // Scale around the crop centre. factor > 1 zooms in, < 1 zooms out.
    void ZoomBy(float factor) { zoom *= factor; Clamp(); }

    void PanTo(float x, float y) { cx = x; cy = y; Clamp(); }

    float U0() const { return cx - Half(); }
    float V0() const { return cy - Half(); }
    float U1() const { return cx + Half(); }
    float V1() const { return cy + Half(); }

    void Reset() { cx = 0.5f; cy = 0.5f; zoom = 1.0f; }
};