#include <uf.h> //файл описаний общих функций
#include <uf_curve.h> //файл описаний функций кривых
#include <uf_csys.h> //файл описаний функций работы с координатами

void circle(float x, float y, float radius) {
	tag_t arc_id, wcs_tag; 
	UF_CURVE_arc_t arc_coords; 

	arc_coords.start_angle = 0.0; 
	arc_coords.end_angle = 360.0 * DEGRA; 
	arc_coords.arc_center[0] = x; //координата центра Х
	arc_coords.arc_center[1] = y; //координата центра Y
	arc_coords.arc_center[2] = 0.; //координата центра Z
	arc_coords.radius = radius; //радиус окружности

	UF_CSYS_ask_wcs(&wcs_tag); //получение абсолютных координат
	//"перенос" абсолютных координат на создаваемую окружность
	UF_CSYS_ask_matrix_of_object(wcs_tag, &arc_coords.matrix_tag);
	UF_CURVE_create_arc(&arc_coords, &arc_id);
}

void line(float startX, float startY, float endX, float endY) {
	tag_t entid = 0; 
	UF_CURVE_line_t line_coords; 

	line_coords.start_point[0] = startX;// X1
	line_coords.start_point[1] = startY;// Y1
	line_coords.start_point[2] = 0.;// Z1
	line_coords.end_point[0] = endX;// X2
	line_coords.end_point[1] = endY;// Y2
	line_coords.end_point[2] = 0.;// Z2

	UF_CURVE_create_line(&line_coords, &entid);
}

void rectangle(float startX, float startY, float sideWitdh) {
	// функция строит квадрат от верхей левой его точки
		
	tag_t entid = 0;
	UF_CURVE_line_t line1_coords;
	UF_CURVE_line_t line2_coords;
	UF_CURVE_line_t line3_coords;
	UF_CURVE_line_t line4_coords;

	//верхняя линия
	line1_coords.start_point[0] = startX;// X1
	line1_coords.start_point[1] = startY;// Y1
	line1_coords.start_point[2] = 0.;// Z1
	line1_coords.end_point[0] = startX + sideWitdh;// X2
	line1_coords.end_point[1] = startY;// Y2
	line1_coords.end_point[2] = 0.;// Z2
	UF_CURVE_create_line(&line1_coords, &entid);

	//правая линия
	line2_coords.start_point[0] = startX + sideWitdh;// X1
	line2_coords.start_point[1] = startY;// Y1
	line2_coords.start_point[2] = 0.;// Z1
	line2_coords.end_point[0] = startX + sideWitdh;// X2
	line2_coords.end_point[1] = startY - sideWitdh;// Y2
	line2_coords.end_point[2] = 0.;// Z2
	UF_CURVE_create_line(&line2_coords, &entid);

	//нижняя линия
	line3_coords.start_point[0] = startX;// X1
	line3_coords.start_point[1] = startY - sideWitdh;// Y1
	line3_coords.start_point[2] = 0.;// Z1
	line3_coords.end_point[0] = startX + sideWitdh;// X2
	line3_coords.end_point[1] = startY - sideWitdh;// Y2
	line3_coords.end_point[2] = 0.;// Z2
	UF_CURVE_create_line(&line3_coords, &entid);

	//левая линия
	line4_coords.start_point[0] = startX;// X1
	line4_coords.start_point[1] = startY;// Y1
	line4_coords.start_point[2] = 0.;// Z1
	line4_coords.end_point[0] = startX;// X2
	line4_coords.end_point[1] = startY - sideWitdh;// Y2
	line4_coords.end_point[2] = 0.;// Z2
	UF_CURVE_create_line(&line4_coords, &entid);
}

void ufusr(char* param, int* retcode, int paramLen)
{
	if (UF_initialize()) return;

	circle(54., 25., 7.5f);

	line(0., 0., 115., 0.);
	line(115., 0., 115., 10.);
	line(115., 10., 107., 10.);
	line(107., 10., 107., 40.);
	line(107., 40., 115., 40.);
	line(115., 40., 115., 50.);
	line(115., 50., 0., 50.);
	line(0., 50., 0., 0.);

	line(27., 0., 27., 10.);
	line(27., 50., 27., 40.);
	line(81., 0., 81., 50.);

	// левый квадрат
	line(10., 10., 10., 40.);
	line(10., 10., 34., 10.);
	line(10., 40., 34., 40.);
	line(34., 40., 34., 10.);

	//центральный большой квадрат
	rectangle(34., 45., 40.);

	//центральный малый квадрат
	rectangle(39., 40., 30.);

	UF_terminate();
}

int ufusr_ask_unload(void) //функция выхода из приложения
{
	return (UF_UNLOAD_IMMEDIATELY);
}

