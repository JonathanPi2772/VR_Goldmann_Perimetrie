import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os


def parse_and_plot(filename, eye=0, subject_id=0 ):
    # 1. Load the dataset
    # The header in the CSV has a comma inside "Points[(PHI,THETA)]", which pandas might split.
    # We read the file and select the first 5 columns which contain the actual data.
    try:
        df = pd.read_csv(filename)
        # Keep valid columns and rename for clarity
        df = df.iloc[:, :5]
        df.columns = ['Longitude', 'SizeIndex', 'Intensity', 'Points', 'NormedValue']
    except Exception as e:
        print(f"Error reading the file: {e}")
        return

    # 2. Parse the coordinate points
    def extract_points(s):
        if pd.isna(s) or s == "[]" or s == "":
            return []
        # Remove brackets and split by semicolon
        content = s.strip("[]")
        pairs = [p for p in content.split(';') if p]

        parsed_points = []
        for p in pairs:
            # Format is usually (x|y) inside parentheses
            p = p.strip("()")
            if '|' in p:
                parts = p.split('|')
                try:
                    # Based on correlation with Longitude:
                    # The first value aligns with the X-axis
                    # The second value aligns with the Y-axis
                    x = float(parts[0])
                    y = float(parts[1])
                    parsed_points.append((x, y))
                except ValueError:
                    continue
        return parsed_points

    df['parsed_points'] = df['Points'].apply(extract_points)

    # 3. Setup the Plot
    plt.figure(figsize=(10, 10))
    ax = plt.gca()

    # Draw standard perimetry grid (Polar style)
    max_r = 90
    # Circles every 10 degrees
    for r in range(10, 100, 10):
        style = '-' if r == 30 else '--'  # Highlight 30 degrees
        alpha = 0.5 if r == 30 else 0.2
        circle = plt.Circle((0, 0), r, color='gray', fill=False, linestyle=style, alpha=alpha)
        ax.add_artist(circle)
        if r % 30 == 0:  # Label 30, 60, 90
            ax.text(r, 0, str(r), color='gray', fontsize=8, ha='center', va='bottom')

    # Radial lines every 30 degrees
    for ang in range(0, 360, 30):
        rad = np.radians(ang)
        x_end = max_r * np.cos(rad)
        y_end = max_r * np.sin(rad)
        plt.plot([0, x_end], [0, y_end], color='gray', linestyle='--', alpha=0.2)
        # Label angles
        ax.text(x_end * 1.05, y_end * 1.05, f"{ang}°", color='gray', fontsize=8, ha='center', va='center')

    # 4. Group by Size and Intensity to plot Isopters
    groups = df.groupby(['SizeIndex', 'Intensity'])

    # Color cycle
    prop_cycle = plt.rcParams['axes.prop_cycle']
    colors = prop_cycle.by_key()['color']
    c_idx = 0

    for (size, intensity), group_data in groups:
        # Collect all points for scatter plot
        all_points = []
        for ptr_list in group_data['parsed_points']:
            all_points.extend(ptr_list)

        if not all_points:
            continue

        # Determine the "Isopter" line (outermost boundary)
        # We find the point with the maximum radius for each Longitude
        line_points = []
        longitudes = sorted(group_data['Longitude'].unique())

        for lng in longitudes:
            rows = group_data[group_data['Longitude'] == lng]
            pts_at_lng = []
            for plist in rows['parsed_points']:
                pts_at_lng.extend(plist)

            if pts_at_lng:
                # Max radius point is considered the outer boundary
                best_p = max(pts_at_lng, key=lambda p: p[0] ** 2 + p[1] ** 2)
                line_points.append(best_p)

        # Plot only if we have a valid line
        if len(line_points) > 2:
            # Close the loop: append the first point to the end
            xs_line = [p[0] for p in line_points]
            ys_line = [p[1] for p in line_points]
            xs_line.append(xs_line[0])
            ys_line.append(ys_line[0])

            color = colors[c_idx % len(colors)]
            label = f"{size} {intensity}"

            # Plot Isopter Line
            plt.plot(xs_line, ys_line, color=color, linewidth=2, label=label)

            # Plot Scatter Points (includes blind spot and repeated measures)
            all_x = [p[0] for p in all_points]
            all_y = [p[1] for p in all_points]
            plt.scatter(all_x, all_y, color=color, s=20, alpha=0.6)

            c_idx += 1

    # Final formatting
    plt.xlim(-100, 100)
    plt.ylim(-100, 100)
    plt.gca().set_aspect('equal', adjustable='box')
    eye_name = ""
    if eye == 1:
        eye_name = "Right"
    elif eye == 2:
        eye_name = "Left"
    plt.title(f'Perimetry Results for Subject {subject_id} | {eye_name} Eye')
    plt.xlabel('X (Degrees)')
    plt.ylabel('Y (Degrees)')

    # Clean up spines
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_visible(False)
    ax.spines['bottom'].set_visible(False)
    plt.xticks([])
    plt.yticks([])

    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    new_filename = filename.split('.csv')[0] + "_empty_plot.png"
    plt.savefig(new_filename)
    # plt.show()

def main():
    for sub in range(8, 10):
        print(os.getcwd())
        path = os.path.join("..", "Measurements", f"Subject{sub}")
        right = os.path.join(path, "Right.csv")
        left = os.path.join(path, "Left.csv")
        print(f"Processing: {right}")
        parse_and_plot(right, eye=1, subject_id=sub)
        print(f"Processing: {left}")
        parse_and_plot(left, eye=2, subject_id=sub)


if __name__ == "__main__":
    main()