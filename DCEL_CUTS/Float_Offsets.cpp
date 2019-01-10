#include "Double_Offsets.h"

FLL<Pdbl> offsetPolygon(FLL<Pdbl> &source, double distance) {
	FLL<Pdbl> adjustments;

	Pdbl last = source.last();
	for (auto next : source) {

		Pdbl segment = next - last;

		Pdbl perp(-segment.Y, segment.X);

		perp.Normalize();

		last = next;
	}
	return FLL<Pdbl>();
}