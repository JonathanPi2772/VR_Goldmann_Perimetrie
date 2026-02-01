import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import CubicSpline
import os


def parse_and_plot_spline(filename):
    # 1. Load Data
    try:
        df = pd.read_csv(filename)
        # Fix column headers if shifted
        df = df.iloc[:, :5]
        df.columns = ['Longitude', 'SizeIndex', 'Intensity', 'Points', 'Val']
    except Exception as e:
        print(f"Error reading file: {e}")
        return

    # 2. Parse Points
    def extract_points(s):
        if pd.isna(s) or s == "[]":
            return []
        s = s.strip("[]")
        pairs = [p for p in s.split(';') if p]
        pts = []
        for p in pairs:
            p = p.strip("()")
            if '|' in p:
                parts = p.split('|')
                try:
                    # x is first, y is second
                    pts.append((float(parts[0]), float(parts[1])))
                except:
                    continue
        return pts

    df['pts_list'] = df['Points'].apply(extract_points)

    # 3. Setup Plot
    plt.figure(figsize=(10, 10))
    ax = plt.gca()

    # Draw polar grid background
    max_r = 90
    for r in range(10, 100, 10):
        style = '-' if r == 30 else '--'
        alpha = 0.5 if r == 30 else 0.2
        circle = plt.Circle((0, 0), r, color='lightgray', fill=False, linestyle=style, alpha=alpha)
        ax.add_artist(circle)
        if r % 30 == 0:
            ax.text(r, 0, str(r), color='gray', fontsize=8, ha='center', va='bottom')

    for ang in range(0, 360, 30):
        rad = np.radians(ang)
        x = max_r * np.cos(rad)
        y = max_r * np.sin(rad)
        plt.plot([0, x], [0, y], color='lightgray', linestyle='--', alpha=0.3)
        ax.text(x * 1.05, y * 1.05, f"{ang}°", color='gray', fontsize=8, ha='center', va='center')

    # 4. Group and Plot
    groups = df.groupby(['SizeIndex', 'Intensity'])
    colors = plt.cm.tab10.colors
    c_idx = 0

    for (size, intensity), group in groups:
        # Sort longitudes to ensure correct ordering for spline
        longitudes = sorted(group['Longitude'].unique())

        # Arrays to store mean points for the spline
        theta_list = []  # Angles
        r_list = []  # Radii (distance from center)

        # Arrays to store scatter points
        all_x = []
        all_y = []

        mean_x_list = []
        mean_y_list = []

        has_data = False

        for lng in longitudes:
            # Get all points for this specific longitude
            rows = group[group['Longitude'] == lng]
            pts = []
            for sublist in rows['pts_list']:
                pts.extend(sublist)

            if not pts:
                continue

            has_data = True

            # Extract x, y
            xs = [p[0] for p in pts]
            ys = [p[1] for p in pts]

            # Add to scatter list
            all_x.extend(xs)
            all_y.extend(ys)

            # Calculate Mean Point
            mean_x = np.mean(xs)
            mean_y = np.mean(ys)

            mean_x_list.append(mean_x)
            mean_y_list.append(mean_y)

            # Convert mean point to polar coordinates (r, theta) for spline interpolation
            # We use the Longitude as the angle theta to ensure monotonic increasing angles
            r = np.sqrt(mean_x ** 2 + mean_y ** 2)
            theta_list.append(lng)
            r_list.append(r)

        if not has_data:
            continue

        color = colors[c_idx % len(colors)]
        label = f"{size} {intensity}"

        # Plot 1: Raw Points (Dots)
        plt.scatter(all_x, all_y, color=color, s=15, alpha=0.3)

        # Plot 2: Mean Points (Triangles)
        plt.scatter(mean_x_list, mean_y_list, color=color, marker='^', s=60,
                    edgecolor='black', linewidth=0.5, label=label + " (Mean)")

        # Plot 3: Spline Curve
        if len(theta_list) > 3:
            # We need to close the loop for the spline.
            # Append the first point to the end (add 360 to angle)
            t = np.array(theta_list)
            r = np.array(r_list)

            t = np.append(t, t[0] + 360)
            r = np.append(r, r[0])

            try:
                # Periodic Cubic Spline
                cs = CubicSpline(t, r, bc_type='periodic')

                # Generate smooth interpolation points
                angles_fine = np.linspace(t[0], t[-1], 200)
                r_fine = cs(angles_fine)

                # Convert back to Cartesian
                x_fine = r_fine * np.cos(np.radians(angles_fine))
                y_fine = r_fine * np.sin(np.radians(angles_fine))

                plt.plot(x_fine, y_fine, color=color, linewidth=2)
            except Exception as e:
                print(f"Spline failed for {label}: {e}")
                # Fallback to linear if spline fails
                plt.plot(mean_x_list + [mean_x_list[0]], mean_y_list + [mean_y_list[0]],
                         color=color, linestyle='--', linewidth=1)

        c_idx += 1

    # Final Formatting
    plt.xlim(-100, 100)
    plt.ylim(-100, 100)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.xlabel('X (Degrees)')
    plt.ylabel('Y (Degrees)')
    plt.title('Perimetry Results- Mean Isopters')
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')

    # Clean axes
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_visible(False)
    ax.spines['bottom'].set_visible(False)
    plt.xticks([])
    plt.yticks([])

    plt.tight_layout()
    # plt.show()
    new_filename = filename.split('.csv')[0] + "_plot_meaned.png"
    plt.savefig(new_filename)

def main():
    for sub in range(1, 6):
        print(os.getcwd())
        path = os.path.join("..", "Measurements", f"Subject{sub}")
        right = os.path.join(path, "Right.csv")
        left = os.path.join(path, "Left.csv")
        print(f"Processing: {right}")
        parse_and_plot_spline(
            right,
            #eye=1, subject_id=sub
        )
        print(f"Processing: {left}")
        parse_and_plot_spline(
            left,
            # eye=2, subject_id=sub
        )


if __name__ == "__main__":
    main()