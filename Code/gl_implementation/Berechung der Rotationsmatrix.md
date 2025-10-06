### Problem-Definition: Rotations-Ambiguität um eine Vektorachse

Die ursprüngliche Berechnungsmethode löst die Rotation eines Vektors **a** zu einem Vektor **b**. Mathematisch findet sie die Rotation, die den Vektor **a** entlang des kürzesten sphärischen Weges (eines Großkreises) auf den Vektor **b** abbildet. Diese Methode definiert jedoch nur zwei von drei möglichen Rotationsfreiheitsgraden. Der dritte Freiheitsgrad – die Rotation des Objekts um die Achse des Vektors **a** selbst – bleibt unbestimmt.

Dieses Phänomen wird als **axiale Rotation** oder "Roll" bezeichnet. In Ihrer Visualisierung führte dies dazu, dass die Objekte zwar korrekt auf den neuen Zielpunkt ausgerichtet wurden, ihre Orientierung um diese Zielachse herum jedoch willkürlich erschien. Das Ziel ist es, diese dritte Rotation zu kontrollieren, um eine konsistente und vorhersagbare Ausrichtung zu gewährleisten.



### Lösungsansatz: Transformation eines vollständigen Koordinatensystems

Die Lösung besteht darin, nicht nur einen einzelnen Vektor, sondern eine vollständige **orthonormale Basis** – also ein lokales 3D-Koordinatensystem – zu rotieren. Eine solche Basis wird durch drei zueinander senkrechte Einheitsvektoren (z.B. `x`, `y`, `z`) definiert, die eine vollständige räumliche Orientierung beschreiben.

Der Prozess erfordert eine **stabile externe Referenz**, um die Ausrichtung des lokalen Systems zu verankern. In der Regel wird hierfür ein globaler "Oben"-Vektor (`world_up`), meist `(0, 1, 0)`, verwendet. Die Rotation wird dann so berechnet, dass sie die Anfangsbasis in die Zielbasis überführt, wobei beide Basen relativ zu diesem globalen Vektor ausgerichtet sind.

---

### Der Berechnungsablauf im Detail

Die Berechnung lässt sich in drei logische Schritte unterteilen: die Konstruktion der Anfangsbasis, die Konstruktion der Zielbasis und die Ableitung der Transformationsmatrix.

#### 1. Konstruktion der Anfangsbasis (Matrix A)

Ausgehend vom initialen Vektor **a** wird eine 3x3-Orientierungsmatrix `Matrix_A` konstruiert. Ihre Spaltenvektoren bilden die Achsen eines rechtshändigen Koordinatensystems.

* **Primärachse (z.B. Z-Achse):** Diese Achse repräsentiert die Hauptausrichtung, also die Richtung des Vektors **a**. Sie wird durch Normalisierung des Vektors gewonnen:
    $$z_A = \frac{a}{\|a\|}$$

* **Sekundärachse (z.B. X-Achse):** Diese Achse wird orthogonal zur Primärachse und zum globalen `world_up`-Vektor bestimmt. Dies geschieht mithilfe des **Kreuzprodukts**, welches per Definition einen Vektor erzeugt, der senkrecht auf seinen beiden Operanden steht.
    $$x_A = \frac{\text{world\_up} \times z_A}{\|\text{world\_up} \times z_A\|}$$
    **Sonderfall (Gimbal Lock):** Sollten `world_up` und `z_A` kollinear (parallel) sein, ist das Ergebnis des Kreuzprodukts ein Nullvektor. In diesem Fall muss eine alternative, nicht-parallele Referenz (z.B. die globale X-Achse `(1, 0, 0)`) zur Berechnung von `x_A` herangezogen werden.

* **Tertiärachse (z.B. Y-Achse):** Die dritte und letzte Achse der Basis muss senkrecht auf den beiden bereits definierten Achsen (`z_A` und `x_A`) stehen. Sie wird ebenfalls durch das Kreuzprodukt ermittelt, um die Rechtshändigkeit des Koordinatensystems zu gewährleisten:
    $$y_A = z_A \times x_A$$
    Eine Normalisierung ist hier nicht mehr nötig, da `z_A` und `x_A` bereits orthogonale Einheitsvektoren sind.

Die resultierenden Vektoren `x_A`, `y_A` und `z_A` bilden die Spalten der **Anfangs-Orientierungsmatrix `Matrix_A`**.

#### 2. Konstruktion der Zielbasis (Matrix B)

Der oben beschriebene Prozess wird identisch für den Zielvektor **b** wiederholt, um die **Ziel-Orientierungsmatrix `Matrix_B`** zu konstruieren, die aus den Spaltenvektoren `x_B`, `y_B` und `z_B` besteht.

#### 3. Ableitung der Rotationsmatrix (R)

Die gesuchte Rotationsmatrix `R` ist die Transformation, die `Matrix_A` in `Matrix_B` überführt. Es gilt die Gleichung:
$$R \cdot \text{Matrix\_A} = \text{Matrix\_B}$$Um `R` zu isolieren, wird die Gleichung mit der Inversen von `Matrix_A` multipliziert:$$R = \text{Matrix\_B} \cdot \text{Matrix\_A}^{-1}$$Eine wesentliche Eigenschaft von **orthonormalen Matrizen** (wie unseren Basismatrizen) ist, dass ihre Inverse identisch mit ihrer **Transponierten** ist. Die Transponierte ist numerisch deutlich effizienter zu berechnen als die Inverse.$$\text{Matrix\_A}^{-1} = \text{Matrix\_A}^T$$Daraus ergibt sich die finale Berechnungsformel:$$R = \text{Matrix\_B} \cdot \text{Matrix\_A}^T$$

### Fazit

Die resultierende Matrix `R` beschreibt die eindeutige Rotation, die eine vollständige räumliche Orientierung von einem Ausgangszustand in einen Endzustand überführt. Indem die Orientierung an einen konstanten globalen Referenzvektor gebunden wird, wird die axiale Rotations-Ambiguität aufgelöst. Das System wird nicht nur auf das Ziel ausgerichtet, sondern behält auch eine konsistente "vertikale" Ausrichtung bei, wodurch der unerwünschte "Roll"-Effekt eliminiert wird.