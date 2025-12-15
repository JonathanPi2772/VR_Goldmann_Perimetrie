# Begründung für eine kleine Studienpopulation

**Ziel der Studie.** Die primäre Fragestellung lautet nicht, wie häufig bestimmte Gesichtsfeldausfälle in der Bevölkerung vorkommen, sondern ob eine VR-basierte, eye-tracking-gestützte Implementierung der *kinetischen* Perimetrie Ergebnisse liefert, die mit der Goldmann-Perimetrie (Goldstandard) ausreichend gut übereinstimmen. Es handelt sich damit um eine **methodische Machbarkeits- und Übereinstimmungsstudie** (Pilot/Feasibility), nicht um eine Prävalenz- oder Wirksamkeitsstudie. Für diesen Zweck ist eine **kleine, gezielt zusammengesetzte Stichprobe** sachgerecht.

## 1) Gepaarte Messungen senken den Stichprobenbedarf

Alle Teilnehmenden dienen als **eigene Kontrolle**: Pro Auge werden VR- und Goldmann-Messungen durchgeführt. Dadurch werden zwischen-personelle Unterschiede (Alter, Pupillendurchmesser, Reaktionsverhalten) weitgehend „herausgekürzt“. Für viele Übereinstimmungsmaße (z. B. Differenzen pro Messpunkt, Isopter-Abstandsmaße, Bland–Altman-Analyse) bestimmt **die Varianz der gepaarten Differenzen** die nötige Fallzahl – nicht die Varianz in der Grundgesamtheit. Das reduziert den benötigten **n** deutlich.

## 2) Hohe Messdichte pro Person

Perimetrie ist **hoch granular**: Jedes Auge liefert Dutzende bis Hunderte von Einzeldatenpunkten (trajektorien- oder isopterbasiert). Zusätzlich sind **Wiederholungsmessungen** pro Person möglich (Test–Retest). Auch wenn diese Messpunkte statistisch nicht vollständig unabhängig sind, erhöht die große Datenmenge pro Auge die Präzision von **innerhalb-Person-Schätzungen** (z. B. mittlerer Isopter-Versatz, Rate richtig erkannter/übersehener Lichtpunkte) und erlaubt robuste Auswertungen mit **gemischten Modellen** oder **cluster-robusten/bootstrapped** Konfidenzintervallen. Kurz: **wenige Personen, viele valide Beobachtungen**.

## 3) Phase „Gesunde“: geringe natürliche Variabilität → kleine n ausreichend

Bei **gesunden Proband\:innen** ist das Gesichtsfeld (nach Korrektur für Alter und Refraktion) **kaum variabel** und liegt in einem engen physiologischen Korridor. Das ist ideal, um

* **systematische Abweichungen** der VR-Implementierung gegenüber Goldmann zu erkennen (Bias, Skalierungs-/Kalibrierungsfehler),
* die **Test–Retest-Stabilität** (Reproduzierbarkeit) zu prüfen,
* technische Parameter (Punktgeschwindigkeit, Schrittweite der Helligkeit, Eye-Tracking-Schwellen) zu optimieren.
  Weil die **erwartete Streuung klein** ist, reichen **wenige gesunde Personen** (mit ggf. 2 Augen und Wiederholungen), um enge Vertrauensintervalle für Bias-Schätzer zu erhalten und klare Go/No-Go-Entscheidungen zu treffen.

## 4) Phase „Erkrankte“: heterogene Defekte gezielt, nicht repräsentativ

Bei **Patient\:innen** ist die Variabilität **gewollt hoch** (z. B. arcuate scotomas, nasaler Step, zentrale/parazentrale Defekte, periphere Einengung). Hier geht es nicht um repräsentative Schätzung von Sensitivität/Spezifität in der Gesamtpopulation, sondern darum, ob die VR-Perimetrie

* **qualitativ** die **Lokalisation, Form und Ausdehnung** individueller Defekte korrekt abbildet (z. B. Überlappung der Isopter, **Dice-Koeffizient**, **Hausdorff-Distanz**),
* **quantitativ** die **Defektschwere** konsistent misst (z. B. Defektfläche in Grad², mittlerer Isopter-Versatz in Grad, Differenzen etablierter Indizes) und
* **längsschnittlich interpretierbar** ist (z. B. Vergleich zur letzten Goldmann-Messung bei Glaukom).
  Eine **kleine, bewusst heterogene Fallserie** (verschiedene Defekttypen, ggf. beidäugig) reicht aus, um die **Bandbreite klinisch relevanter Situationen** abzudecken und die Leistungsfähigkeit der Methode demonstrativ und statistisch nachvollziehbar zu belegen.

## 5) Getrennte qualitative und quantitative Endpunkte

* **Qualitativ:** Übereinstimmung der **Defektmorphologie** (Position, Kontur, Größe). Kennzahlen wie Isopter-Überlappung (Dice/Jaccard), Kontur-Abstände, Trefferrate für als „blind“ klassifizierte Regionen sind selbst bei kleinem **n** informativ, weil sie **innerhalb desselben Auges** zwischen VR und Goldmann verglichen werden.
* **Quantitativ:** **Differenzen** kontinuierlicher Maße (z. B. Isopter-Radius in definierten Meridianen, Defektflächen) erlauben **Bland–Altman-Analysen** und **Äquivalenz-/Nichtunterlegenheitsprüfungen** gegen eine **vorab definierte klinische Toleranz**. Die Präzision hängt primär von der **innerhalb-Auge-Streuung** ab, nicht von großen Stichproben.

## 6) Ethik und Proportionalität

Die VR-Implementierung ist **neu**; daher ist es ethisch geboten, zunächst **mit möglichst geringer Personenbelastung** zu evaluieren:

1. **Gesunde** zur technischen Validierung bei minimalem Risiko,
2. danach **erkrankte Patient\:innen** für den Vergleich in realen Defektkonstellationen.
   Dieses **Stufenmodell** minimiert Aufwand und Risiko, bevor ggf. in einer nachfolgenden, größeren Validierungsstudie eine breitere Generalisierbarkeit geprüft wird.

## 7) Statistische Verfahren, die kleine n unterstützen

* **Bland–Altman** (Bias, 95 %-Limits of Agreement) für kontinuierliche Differenzen,
* **Intraklassenkorrelation (ICC)** für Reproduzierbarkeit und Übereinstimmung,
* **Dice/Jaccard** und **Hausdorff-Distanz** für Flächen/Konturen,
* **gemischte Modelle** mit Clustering nach Person/Auge und **Bootstrap** zur robusten Inferenz,
* **prä-definierte Äquivalenzgrenzen** (klinisch motiviert) statt klassischer Überlegenheits-Tests.
  Diese Methoden nutzen die **gepaarte Struktur** und die **vielen Messpunkte pro Auge** optimal aus – und sind für Pilot-/Machbarkeitsfragen ausreichend aussagekräftig.

## 8) Praktischer Mehrwert trotz kleinem n

Selbst mit **n < 10 pro Gruppe** lassen sich klinisch relevante Fragen beantworten:

* Erkennt die VR-Perimetrie **die gleichen Defekte** an der **gleichen Stelle**?
* Liegen **Größe/Schwere** der Defekte **innerhalb definierter Toleranzen** zur Goldmann-Messung?
* Ist die **Messwiederholbarkeit** (Test–Retest) auf dem erwarteten Niveau?
* Bevorzugen Patient\:innen die VR-Variante (Fragebogen), und ist der **Zeitbedarf** konkurrenzfähig?

**Fazit:** Für eine **methodische Übereinstimmungs- und Machbarkeitsprüfung** mit gepaarten Messungen, hoher Messdichte pro Auge, geringer natürlicher Variabilität bei Gesunden und gezielter Heterogenität bei Erkrankten ist eine **kleine Studienpopulation fachlich, statistisch und ethisch angemessen**. Größere Stichproben sind erst dann erforderlich, wenn im Anschluss **Generalisierbarkeit, Grenzfälle** oder **klinische Entscheidungsgrenzen** in verschiedenen Subpopulationen formal quantifiziert werden sollen.
