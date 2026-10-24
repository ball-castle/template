#!/usr/bin/env python3
"""Render every PDF page with Poppler and create small contact sheets for navigation."""
import argparse
from pathlib import Path
import shutil
import subprocess
from PIL import Image, ImageOps, ImageDraw

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('pdf', type=Path)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--poppler', default=shutil.which('pdftoppm'))
    ap.add_argument('--dpi', type=int, default=130)
    args = ap.parse_args()
    if not args.poppler:
        raise SystemExit('Pass --poppler /path/to/pdftoppm')
    args.out.mkdir(parents=True, exist_ok=True)
    subprocess.run([args.poppler,'-r',str(args.dpi),'-png',str(args.pdf),str(args.out/'page')],check=True)
    images = sorted(args.out.glob('page-*.png'))
    for start in range(0,len(images),6):
        sheet = Image.new('RGB',(1200,1152),'#aaaaaa')
        draw = ImageDraw.Draw(sheet)
        for index,path in enumerate(images[start:start+6]):
            thumb = Image.open(path).convert('RGB')
            thumb.thumbnail((392,550))
            x, y = (index%3)*400, (index//3)*576
            sheet.paste(thumb,(x+(400-thumb.width)//2,y+22))
            draw.text((x+8,y+5),path.stem,fill='black')
        sheet.save(args.out/f'contact-{start//6+1:02d}.png')
    print(f'Rendered {len(images)} pages to {args.out}; inspect each page at full size.')

if __name__ == '__main__':
    main()
