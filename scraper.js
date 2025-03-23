// scraper.js
const puppeteer = require('puppeteer');

(async () => {
    const url = process.argv[2];
    if (!url) {
        console.error("No URL provided.");
        process.exit(1);
    }
    const browser = await puppeteer.launch({ args: ['--no-sandbox'] });
    const page = await browser.newPage();
    await page.goto(url, { waitUntil: 'networkidle2' });
    const content = await page.content();
    console.log(content);
    await browser.close();
})();

