#!/usr/bin/env python3
"""Export current DOCX files and bind each successful PDF to its source hash."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

root = Path(__file__).resolve().parents[1]

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('docx', type=Path, nargs='*')
    ap.add_argument('--soffice', default=shutil.which('libreoffice') or shutil.which('soffice'))
    args = ap.parse_args()
    paths = args.docx or sorted((root/'板子打印版').glob('*.docx'))
    if not paths:
        raise SystemExit('No DOCX files')
    for path in paths:
        path = path.resolve()
        pdf = path.with_suffix('.pdf')
        if os.name == 'nt':
            subprocess.run(['powershell','-NoProfile','-File',str(root/'scripts/export_word.ps1'),
                            '-Docx',str(path),'-Pdf',str(pdf)],check=True)
        else:
            if not args.soffice:
                raise SystemExit('LibreOffice is required; pass --soffice explicitly')
            started=time.time()
            subprocess.run([args.soffice,'--headless','--convert-to','pdf','--outdir',str(path.parent),str(path)],check=True)
            if not pdf.exists() or pdf.stat().st_mtime < started-2:
                raise RuntimeError('Exporter did not create a fresh PDF')
            sha=lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
            binding={'engine':'LibreOffice headless','docx_sha256':sha(path),'pdf_sha256':sha(pdf)}
            pdf.with_name(pdf.name+'.binding.json').write_text(json.dumps(binding,indent=2)+'\n',encoding='utf-8')

if __name__=='__main__':
    main()
