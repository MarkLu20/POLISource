// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct BMapPixel {
	int X, Y;

	BMapPixel() {
		X = Y = 0;
	}
	BMapPixel(int x, int y) {
		X = x;
		Y = y;
	}
};

struct BMapPoint {
	double longitude;
	double latitude;

	BMapPoint() {
		longitude = latitude = 0;
	}
	BMapPoint(double x, double y) {
		longitude = x;
		latitude = y;
	}
};

const static double pi = 3.1415926535897932384626433832795;
const static double a = 6378245.0; //卫星椭球坐标投影到平面坐标系的投影因子
const static double ee = 0.00669342162296594323; //椭球的偏心率
class  BMapCoordinate
{
public:	
	/**
	* 经纬度坐标(lng， lat)转平面坐标（pointX, pointY）
	* 输入Point类，返回Pixel类
	**/
	static BMapPixel lngLatToPoint(double x, double y)
	{
		BMapPoint point = zb(BMapPoint(x, y));
		return BMapPixel((int)point.longitude, (int)point.latitude);
	}

	/**
	* 平面坐标（pointX, pointY）转经纬度坐标(lng， lat)
	* 输入Pixel类，返回Point类
	**/
	static BMapPoint pointToLngLat(int x, int y)
	{
		BMapPoint pixel = BMapPoint(x, y);
		BMapPoint point = Fb(pixel);
		return point;
	}

	//平面坐标（pointX, pointY）转瓦片坐标（tileX， tileY）
	static double PixelToTile(int pixel, int Level)
	{
		double n = pow(2, Level - 18);
		return pixel * n / 256;
	}

	//瓦片（tileX， tileY）的像素坐标（pixelX, pixelY）转平面坐标（pointX, pointY）
	static int TileAndPixelToPoint(int tile, double pixel, int Level)
	{
		pixel = pixel > 0 ? pixel : -pixel;
		int n = 1 << (Level - 18);
		return (int)((tile * 256 + pixel) / n);
	}

	/*
	* 地球坐标转火星坐标
	* @param wgLat  地球坐标
	* @param wgLon
	* mglat,mglon 火星坐标
	*/
	static BMapPoint wgs84_To_gcj02(double wgLon, double wgLat)
	{
		double dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
		double dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
		double radLat = wgLat / 180.0 * pi;
		double magic = sin(radLat);
		magic = 1 - ee * magic * magic;
		double sqrtMagic = sqrt(magic);
		dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
		dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
		double mgLat = wgLat + dLat;
		double mgLon = wgLon + dLon;

		return BMapPoint(mgLon, mgLat);
	}
	/*
	* 火星转地球
	*/
	static BMapPoint gcj02_To_wgs84(double wgLon, double wgLat)
	{
		double dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
		double dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
		double radLat = wgLat / 180.0 * pi;
		double magic = sin(radLat);
		magic = 1 - ee * magic * magic;
		double sqrtMagic = sqrt(magic);
		dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
		dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
		double mgLat = wgLat + dLat;
		double mgLon = wgLon + dLon;		
		return BMapPoint(wgLon * 2 - mgLon, wgLat * 2 - mgLat);
	}

	static double transformLat(double x, double y)
	{
		double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(FMath::Abs(x));
		ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
		ret += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
		ret += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;
		return ret;
	}

	static double transformLon(double x, double y)
	{
		double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(FMath::Abs(x));
		ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
		ret += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
		ret += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0 * pi)) * 2.0 / 3.0;
		return ret;
	}

	/*
	** 火星坐标系 (GCJ-02) 与百度坐标系 (BD-09) 的转换算法 将 GCJ-02 坐标转换成 BD-09 坐标
	*/
	static BMapPoint gcj02_To_Bd09(double gg_lon, double gg_lat)
	{
		double x = gg_lon, y = gg_lat;
		double z = sqrt(x * x + y * y) + 0.00002 * sin(y * 3.1415926535897932384626433832795 * 3000.0 / 180.0);
		double theta = atan2(y, x) + 0.000003 * cos(x * 3.1415926535897932384626433832795 * 3000.0 / 180.0);
		double bd_lon = z * cos(theta) + 0.0065;
		double bd_lat = z * sin(theta) + 0.006;
		return BMapPoint(bd_lon, bd_lat);
	}

	/*
	** 火星坐标系(GCJ-02) 与百度坐标系(BD-09) 的转换算法 将 BD-09 坐标转换成 GCJ-02 坐标
	*/
	static BMapPoint Bd09_To_gcj02_(double bd_lon, double bd_lat)
	{
		double x = bd_lon - 0.0065;
		double y = bd_lat - 0.006;
		double z = sqrt(x * x + y * y) - 0.00002 * sin(y * 3.1415926535897932384626433832795 * 3000.0 / 180.0);
		double theta = atan2(y, x) - 0.000003 * cos(x * 3.1415926535897932384626433832795 * 3000.0 / 180.0);
		double gg_lng = z * cos(theta);
		double gg_lat = z * sin(theta);
		return BMapPoint(gg_lng, gg_lat);
	}
private:
	static const BMapPoint Fb(BMapPoint a)
	{
		double rG[] = { 1.289059486E7, 8362377.87, 5591021, 3481989.83, 1678043.12, 0 };
		double pP[][10] = { { 1.410526172116255E-8, 8.98305509648872E-6, -1.9939833816331, 200.9824383106796, -187.2403703815547, 91.6087516669843, -23.38765649603339, 2.57121317296198, -0.03801003308653, 1.73379812E7 }
			,{ -7.435856389565537E-9, 8.983055097726239E-6, -0.78625201886289, 96.32687599759846, -1.85204757529826, -59.36935905485877, 47.40033549296737, -16.50741931063887, 2.28786674699375, 1.026014486E7 }
			,{ -3.030883460898826E-8, 8.98305509983578E-6, 0.30071316287616, 59.74293618442277, 7.357984074871, -25.38371002664745, 13.45380521110908, -3.29883767235584, 0.32710905363475, 6856817.37 }
			,{ -1.981981304930552E-8, 8.983055099779535E-6, 0.03278182852591, 40.31678527705744, 0.65659298677277, -4.44255534477492, 0.85341911805263, 0.12923347998204, -0.04625736007561, 4482777.06 }
			,{ 3.09191371068437E-9, 8.983055096812155E-6, 6.995724062E-5, 23.10934304144901, -2.3663490511E-4, -0.6321817810242, -0.00663494467273, 0.03430082397953, -0.00466043876332, 2555164.4 }
		,{ 2.890871144776878E-9, 8.983055095805407E-6, -3.068298E-8, 7.47137025468032, -3.53937994E-6, -0.02145144861037, -1.234426596E-5, 1.0322952773E-4, -3.23890364E-6, 826088.5 } };
		BMapPoint b = BMapPoint(a.longitude > 0 ? a.longitude : -a.longitude, a.latitude > 0 ? a.latitude : -a.latitude);

		double c[10] = { 0 };
		bool processed = false;
		for (int d = 0; d < 6; ++d)
		{
			if (b.latitude >= rG[d])
			{
				for (int m = 0; m < 10; m++)
					c[m] = pP[d][m];
				processed = true;
				break;
			}
		}
		a = oK(a, c, processed ? 10 : 0);
		return a;
	}

	static const BMapPoint& zb(BMapPoint point)
	{
		double c[10] = { 0 };
		point.longitude = MD(point.longitude, -180, 180);
		point.latitude = QD(point.latitude, -74, 74);
		BMapPoint b(point.longitude, point.latitude);

		int Bu[] = { 75, 60, 45, 30, 15, 0 };
		double oG[][10] =
		{ { -0.0015702102444, 111320.7020616939, 1704480524535203, -10338987376042340, 26112667856603880, -35149669176653700, 26595700718403920, -10725012454188240, 1800819912950474, 82.5 }
			,{ 8.277824516172526E-4, 111320.7020463578, 6.477955746671607E8, -4.082003173641316E9, 1.077490566351142E10, -1.517187553151559E10, 1.205306533862167E10, -5.124939663577472E9, 9.133119359512032E8, 67.5 }
			,{ 0.00337398766765, 111320.7020202162, 4481351.045890365, -2.339375119931662E7, 7.968221547186455E7, -1.159649932797253E8, 9.723671115602145E7, -4.366194633752821E7, 8477230.501135234, 52.5 }
			,{ 0.00220636496208, 111320.7020209128, 51751.86112841131, 3796837.749470245, 992013.7397791013, -1221952.21711287, 1340652.697009075, -620943.6990984312, 144416.9293806241, 37.5 }
			,{ -3.441963504368392E-4, 111320.7020576856, 278.2353980772752, 2485758.690035394, 6070.750963243378, 54821.18345352118, 9540.606633304236, -2710.55326746645, 1405.483844121726, 22.5 }
		,{ -3.218135878613132E-4, 111320.7020701615, 0.00369383431289, 823725.6402795718, 0.46104986909093, 2351.343141331292, 1.58060784298199, 8.77738589078284, 0.37238884252424, 7.45 } };

		bool processed = false;
		for (int d = 0; d < 6; d++)
		{
			if (b.latitude >= Bu[d])
			{
				for (int m = 0; m < 10; m++)
					c[m] = oG[d][m];
				processed = true;
				break;
			}
		}
		if (!processed)
		{
			for (int d = 0; d < 6; d++)
				if (b.latitude <= -Bu[d])
				{
					for (int m = 0; m < 10; m++)
						c[m] = oG[d][m];
					processed;
					break;
				}
		}
		point = oK(point, c, processed ? 10 : 0);
		return point;
	}

	static double MD(double a, double b, double c)
	{
		for (; a > c;) a -= c - b;
		for (; a < b;) a += c - b;
		return a;
	}

	static double QD(double a, double b, double c)
	{
		if (b != 0)
			a = a > b ? a : b;
		if (c != 0)
			a = a < c ? a : c;
		return a;
	}

	static BMapPoint oK(BMapPoint a, double b[], int length)
	{
		if (length > 0)
		{
			double c = b[0] + b[1] * (a.longitude < 0 ? -a.longitude : a.longitude);
			double d = (a.latitude < 0 ? -a.latitude : a.latitude) / b[9];
			d = b[2] + b[3] * d + b[4] * d * d + b[5] * d * d * d + b[6] * d * d * d * d + b[7] * d * d * d * d * d + b[8] * d * d * d * d * d * d;
			c = c * (0 > a.longitude ? -1 : 1);
			d = d * (0 > a.latitude ? -1 : 1);
			return BMapPoint(c, d);
		}
		return BMapPoint(0, 0);
	}
};
