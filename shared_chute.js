function generateSharedChutes() {
  const horizontalChutes = [];
  const verticalChutes = [];
  for (let i = 0; i < 81; i++) {
    const iRow = Math.floor(i / 9);
    const iCol = i % 9;
    const iHorizontalChute = Math.floor(iRow / 3);
    const iVertChute = Math.floor(iCol / 3) + 3;

    horizontalChutes.push(iHorizontalChute);
    verticalChutes.push(iVertChute)
  }
  return { horizontalChutes, verticalChutes }

}
const result = generateSharedChutes();

console.log(`{${result.horizontalChutes.map((val) => `${val}`).join(',')} },`)
console.log(`{${result.verticalChutes.map((val) => `${val}`).join(',')} },`)
