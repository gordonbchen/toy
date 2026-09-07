"""Render bold black ℓᵖ unit-ball outlines for p = ∞, 2, 1, and 1/2.

For 0 < p < 1 these are quasi-norm balls, with inward-curving sides.

Run with: .venv/bin/python lp_avatar.py
Dependencies: numpy, pillow
"""

from pathlib import Path

import numpy as np
from PIL import Image, ImageDraw


def render(output: str = "lp_avatar.png", size: int = 2048) -> None:
    scale = 3  # Supersampling keeps the curves smooth at avatar sizes.
    canvas = Image.new("RGB", (size * scale, size * scale), "white")
    draw = ImageDraw.Draw(canvas)
    center = size * scale / 2
    radius = size * scale * 0.335  # Keep square corners inside a circular crop.
    stroke = round(size * scale * 0.018)
    theta = np.linspace(0, 2 * np.pi, 8192, endpoint=False)
    directions = np.stack((np.cos(theta), np.sin(theta)), axis=1)
    directions[np.abs(directions) < 1e-14] = 0

    # Every contour has the same unit radius along the coordinate axes.
    draw.rectangle(
        (round(center - radius - stroke / 2),) * 2
        + (round(center + radius + stroke / 2),) * 2,
        outline="black", width=stroke,
    )
    for p in (2, 1, 0.5):
        lengths = np.sum(np.abs(directions) ** p, axis=1) ** (1 / p)
        points = center + radius * directions / lengths[:, None]
        vertices = [tuple(point) for point in np.rint(points).astype(int)]
        draw.line(vertices + [vertices[0]], fill="black", width=stroke, joint="curve")

    canvas = canvas.resize((size, size), Image.Resampling.LANCZOS)
    canvas.save(output)
    print(f"Saved {Path(output).resolve()} ({size} × {size})")


if __name__ == "__main__":
    render()
