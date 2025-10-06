from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle, Image as RLImage, PageBreak
from reportlab.lib.enums import TA_CENTER, TA_JUSTIFY, TA_LEFT
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
import os

logo_path = "Logo_UKHD.png"
font_path = "Archivo_Font/ttf/"
file_path = "Patientenfragebogen.pdf"

pdfmetrics.registerFont(TTFont("Archivo", f"{font_path}Archivo-Regular.ttf"))
pdfmetrics.registerFont(TTFont("ArchivoBold", f"{font_path}Archivo-Bold.ttf"))
pdfmetrics.registerFont(TTFont("ArchivoSemiBold", f"{font_path}Archivo-SemiBold.ttf"))

doc = SimpleDocTemplate(file_path, pagesize=A4, rightMargin=40, leftMargin=40, topMargin=40, bottomMargin=40)

styles = getSampleStyleSheet()
# styles.add(ParagraphStyle(name='Title', fontName='Archivo', alignment=TA_CENTER, fontSize=16, spaceAfter=12, leading=18))
styles.add(ParagraphStyle(name='Head', fontName='Archivo', alignment=TA_CENTER, fontSize=13, spaceAfter=8, leading=15))
styles.add(ParagraphStyle(name='Header', fontName='ArchivoBold', alignment=TA_CENTER, fontSize=13, spaceAfter=8, leading=15))
styles.add(ParagraphStyle(name='Question', fontName='ArchivoSemiBold', alignment=TA_LEFT, fontSize=11, spaceAfter=4, leading=13))
styles.add(ParagraphStyle(name='Answer', fontName='Archivo', alignment=TA_LEFT, fontSize=11, spaceAfter=2, leading=13, leftIndent=15))

elements = []

# Logo (Platzhalter oder Kliniklogo)
# if os.path.exists(logo_path):
    # elements.append(
logo = RLImage(logo_path, width=80, height=80, hAlign="LEFT")
    # )
    # elements.append(Spacer(1, 10))

# Titel
title_block = [Paragraph("Patienten-Fragebogen", styles['Title']),
               Paragraph("Kinetische Perimetrie nach Goldmann mittels VR", styles['Head'])]

table_title = Table([[title_block, logo]], colWidths=[400, 80])
table_title.setStyle(TableStyle([("VALIGN", (0,0), (-1,-1), "MIDDLE")]))
elements.append(table_title)
elements.append(Spacer(1, 20))

# Allgemeine Angaben
elements.append(Paragraph("Allgemeine Angaben", styles['Header']))
data_allg = [
    # ["Name (optional, Initialen):", "_______________________________"],
    ["Alter:", "______ Jahre"],
    ["Geschlecht:", "□ weiblich    □ männlich    □ divers"],
    ["Datum:", "__ / __ / ____"],
]
t_allg = Table(data_allg, colWidths=[180, 300])
t_allg.setStyle(TableStyle([("VALIGN", (0,0), (-1,-1), "TOP")]))
elements.append(t_allg)
elements.append(Spacer(1, 20))

# Fragen
elements.append(Paragraph("Fragen zur Untersuchung", styles['Header']))
elements.append(Spacer(1, 10))

def frage(nr, text, antworten):
    elements.append(Paragraph(f"<b>{nr}. {text}</b>", styles['Question']))
    answers = []
    col_widths = []
    for a in antworten:
        ans_txt = f"☑ {a}"
        answers.append(Paragraph(ans_txt, styles['Answer']))
        # col_widths.append(int(len(ans_txt)*5 + 25))
        col_widths.append((500 // len(antworten)))

    t_ans = Table([answers], colWidths=col_widths)
    t_ans.setStyle(TableStyle([("VALIGN", (0, 0), (-1, -1), "TOP")]))
    elements.append(t_ans)
    elements.append(Spacer(1, 15))

frage(1, "Wie oft wurde bei Ihnen bereits eine Gesichtsfelduntersuchung durchgeführt?",
      ["noch nie", "1 Mal", "2–5 Mal", "mehr als 5 Mal"])
frage(2, "Haben Sie zum ersten Mal eine VR-Brille verwendet?",
      ["Ja", "Nein"])
frage(3, "Wie empfanden Sie die Nutzung der VR-Brille?",
      ["einfach", "mittel", "schwierig"])
frage(4, "Welches Verfahren empfanden Sie angenehmer?",
      ["Normale Untersuchung (Standard-Perimetrie)", "Untersuchung mit VR-Brille", "Beide gleich angenehm"])
frage(5, "Hatten Sie während der Untersuchung Beschwerden (z. B. Schwindel, Übelkeit, Kopfschmerzen)?",
      ["Nein", "Ja, folgende: __________________________"])

# Datenschutz & Unterschrift – direkt unten auf derselben Seite
elements.append(Spacer(1, 14))
elements.append(Paragraph("Einverständnis & Datenschutz", styles['Header']))
ds = ("Ich bestätige, dass ich über die Durchführung der Untersuchung informiert wurde und mit der Teilnahme einverstanden bin. "
      "Die erhobenen Daten werden ausschließlich zu wissenschaftlichen und medizinischen Zwecken genutzt und vertraulich behandelt.")
elements.append(Paragraph(ds, styles['Answer']))
elements.append(Spacer(1, 25))

data_sign = [
    ["Unterschrift Patient/in:", "___________________________"],
    ["Unterschrift Untersucher/in:", "___________________________"],
    ["Ort/Datum:", "___________________________"],
]
t_sign = Table(data_sign, colWidths=[200, 250])
t_sign.setStyle(TableStyle([("VALIGN", (0,0), (-1,-1), "TOP")]))
elements.append(t_sign)

# PDF erzeugen
doc.build(elements)

print("PDF erstellt:", file_path)
