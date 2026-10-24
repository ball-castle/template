"""A rebuilt DOCX must not let an older, same-length PDF pass the print gate."""
import contextlib
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import tempfile
import unittest
from docx import Document
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import A4

spec = importlib.util.spec_from_file_location('check_print', Path(__file__).resolve().parents[1]/'scripts/check_print.py')
gate = importlib.util.module_from_spec(spec)
spec.loader.exec_module(gate)

class PrintBindingTests(unittest.TestCase):
    def test_same_page_count_stale_pdf_is_rejected(self):
        with tempfile.TemporaryDirectory() as temp:
            root=Path(temp); gate.ROOT=root
            (root/'core').mkdir()
            source=root/'core/example.hpp'; source.write_text('// v2\n',encoding='utf-8')
            doc=Document(); doc.styles.add_style('Code',1)
            doc.add_paragraph('// v2',style='Code'); doc.save(root/'test.docx')
            pdf=canvas.Canvas(str(root/'test.pdf'),pagesize=A4)
            pdf.drawString(50,780,'X01 previous implementation v1 - same page count but stale source')
            pdf.save()
            manifest={'planned_pages':1,'modules':[{'id':'X01','source':'core/example.hpp',
                'sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'start_page':1}]}
            path=root/'test.manifest.json';path.write_text(json.dumps(manifest))
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertFalse(gate.check(path))

if __name__=='__main__':
    unittest.main()
