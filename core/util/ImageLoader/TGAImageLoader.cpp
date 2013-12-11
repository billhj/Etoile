/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TGAImageLoader.cpp
* @brief 
* @date 1/2/2011
*/

#include "TGAImageLoader.h"

namespace Etoile
{

#pragma pack(push, 1)
	typedef struct
	{
		unsigned char id_lenght;          /* size of image id */
		unsigned char colormap_type;      /* 1 is has a colormap */
		unsigned char image_type;         /* compression type */

		short	cm_first_entry;       /* colormap origin */
		short	cm_length;            /* colormap length */
		unsigned char cm_size;            /* colormap size */

		short	x_origin;             /* bottom left x coord origin */
		short	y_origin;             /* bottom left y coord origin */

		short	width;                /* picture width (in pixels) */
		short	height;               /* picture height (in pixels) */

		unsigned char pixel_depth;        /* bits per pixel: 8, 16, 24 or 32 */
		unsigned char image_descriptor;   /* 24 bits = 0x00; 32 bits = 0x80 */

	} tga_header_t;
#pragma pack(pop)




	void
		ReadTGA8bits (FILE *fp, unsigned char *colormap, Image& image)
	{
		int i;
		unsigned char color;

		for (i = 0; i < image.getWidth() * image.getHeight(); ++i)
		{
			/* read index color byte */
			color = (unsigned char)fgetc (fp);

			/* convert to RGB 24 bits */
			float blue = colormap[(color * 3) + 0] / float(255);
			float green = colormap[(color * 3) + 1]/ float(255);
			float red = colormap[(color * 3) + 2]/ float(255);
			float alpha = 1;
			Vec4f color(red,green,blue,alpha);
			image.getData().push_back(color);
		}
	}


	void
		ReadTGA16bits (FILE *fp, Image& image)
	{
		int i;
		unsigned short color;

		for (i = 0; i < image.getWidth() * image.getHeight(); ++i)
		{
			/* read color word */
			color = fgetc (fp) + (fgetc (fp) << 8);

			/* convert BGR to RGB */
			float red = (unsigned char)(((color & 0x7C00) >> 10) << 3)/ float(255);
			float green = (unsigned char)(((color & 0x03E0) >>  5) << 3)/ float(255);
			float blue = (unsigned char)(((color & 0x001F) >>  0) << 3)/ float(255);
			float alpha = 1;
			Vec4f color(red,green,blue,alpha);
			image.getData().push_back(color);
		}
	}


	void
		ReadTGA24bits (FILE *fp, Image& image)
	{
		int i;

		for (i = 0; i < image.getWidth() * image.getHeight(); ++i)
		{
			/* read and convert BGR to RGB */
			float blue = (unsigned char)fgetc (fp)/ float(255);
			float green = (unsigned char)fgetc (fp)/ float(255);
			float red = (unsigned char)fgetc (fp)/ float(255);
			float alpha = 1;
			Vec4f color(red,green,blue,alpha);
			image.getData().push_back(color);
		}
	}


	void
		ReadTGA32bits (FILE *fp, Image& image)
	{
		int i;

		for (i = 0; i < image.getWidth() * image.getHeight(); ++i)
		{
			/* read and convert BGRA to RGBA */
			float blue = (unsigned char)fgetc (fp)/ float(255);
			float green = (unsigned char)fgetc (fp)/ float(255);
			float red = (unsigned char)fgetc (fp)/ float(255);
			float alpha = (unsigned char)fgetc (fp)/ float(255);
			Vec4f color(red,green,blue,alpha);
			image.getData().push_back(color);
		}
	}


	void
		ReadTGAgray8bits (FILE *fp, Image& image)
	{
		int i;

		for (i = 0; i < image.getWidth() * image.getHeight(); ++i)
		{
			/* read grayscale color byte */
			float gray = fgetc (fp)/ float(255);
			Vec4f color(gray,gray,gray,gray);
			image.getData().push_back(color);
		}
	}


	void
		ReadTGAgray16bits (FILE *fp, Image& image)
	{
		int i;

		for (i = 0; i < image.getWidth() * image.getHeight(); ++i)
		{
			/* read grayscale color + alpha channel bytes */
			float gray = fgetc (fp)/ float(255);
			float alpha = fgetc (fp)/ float(255);
			Vec4f color(gray,gray,gray,alpha);
			image.getData().push_back(color);
		}
	}


	void
		ReadTGA8bitsRLE (FILE *fp, unsigned char *colormap, Image& image)
	{
		int i, size;
		unsigned char color;
		unsigned char packet_header;
		int j = 0;
		while (j < image.getWidth() * image.getHeight() * 3)
		{
			/* read first byte */
			packet_header = (unsigned char)fgetc (fp);
			size = 1 + (packet_header & 0x7f);

			if(size != image.getWidth() * image.getHeight()) std::cout<<"wrong size ReadTGA8bitsRLE"<<std::endl;
			if (packet_header & 0x80)
			{
				/* run-length packet */
				color = (unsigned char)fgetc (fp);

				for (i = 0; i < size; ++i, j = j + 3)
				{
					float red = colormap[(color * 3) + 2]/ float(255);
					float green = colormap[(color * 3) + 1]/ float(255);
					float blue = colormap[(color * 3) + 0]/ float(255);
					float alpha = 1;
					Vec4f color(red,green,blue,alpha);
					image.getData().push_back(color);
				}
			}
			else
			{
				/* non run-length packet */
				for (i = 0; i < size; ++i, j = j + 3)
				{
					color = (unsigned char)fgetc (fp);

					float red = colormap[(color * 3) + 2]/ float(255);
					float green = colormap[(color * 3) + 1]/ float(255);
					float blue = colormap[(color * 3) + 0]/ float(255);
					float alpha = 1;
					Vec4f color(red,green,blue,alpha);
					image.getData().push_back(color);
				}
			}
		}
	}


	void
		ReadTGA16bitsRLE (FILE *fp, Image& image)
	{
		int i, size;
		unsigned short color;
		unsigned char packet_header;

		int j = 0;
		while (j < image.getWidth() * image.getHeight() * 3)
		{
			/* read first byte */
			packet_header = fgetc (fp);
			size = 1 + (packet_header & 0x7f);

			if (packet_header & 0x80)
			{
				/* run-length packet */
				color = fgetc (fp) + (fgetc (fp) << 8);

				for (i = 0; i < size; ++i, j = j + 3)
				{
					float red = (unsigned char)(((color & 0x7C00) >> 10) << 3)/ float(255);
					float green = (unsigned char)(((color & 0x03E0) >>  5) << 3)/ float(255);
					float blue = (unsigned char)(((color & 0x001F) >>  0) << 3)/ float(255);
					float alpha = 1;
					Vec4f color(red,green,blue,alpha);
					image.getData().push_back(color);
				}
			}
			else
			{
				/* non run-length packet */
				for (i = 0; i < size; ++i, j = j + 3)
				{
					color = fgetc (fp) + (fgetc (fp) << 8);

					float red = (unsigned char)(((color & 0x7C00) >> 10) << 3)/ float(255);
					float green = (unsigned char)(((color & 0x03E0) >>  5) << 3)/ float(255);
					float blue = (unsigned char)(((color & 0x001F) >>  0) << 3)/ float(255);
					float alpha = 1;
					Vec4f color(red,green,blue,alpha);
					image.getData().push_back(color);
				}
			}
		}
	}


	void
		ReadTGA24bitsRLE (FILE *fp, Image& image)
	{
		int i, size;
		unsigned char rgb[3];
		unsigned char packet_header;

		int j = 0;
		while (j < image.getWidth() * image.getHeight() * 3)
		{
			/* read first byte */
			packet_header = (unsigned char)fgetc (fp);
			size = 1 + (packet_header & 0x7f);

			if (packet_header & 0x80)
			{
				/* run-length packet */
				fread (rgb, sizeof (unsigned char), 3, fp);

				for (i = 0; i < size; ++i, j = j + 3)
				{
					float red = rgb[2]/ float(255);
					float green = rgb[1]/ float(255);
					float blue = rgb[0]/ float(255);
					float alpha = 1;
					Vec4f color(red,green,blue,alpha);
					image.getData().push_back(color);
				}
			}
			else
			{
				/* non run-length packet */
				for (i = 0; i < size; ++i, j = j + 3)
				{
					float blue = (unsigned char)fgetc (fp)/ float(255);
					float green = (unsigned char)fgetc (fp)/ float(255);
					float red = (unsigned char)fgetc (fp)/ float(255);
					float alpha = 1;
					Vec4f color(red,green,blue,alpha);
					image.getData().push_back(color);
				}
			}
		}
	}


	void
		ReadTGA32bitsRLE (FILE *fp, Image& image)
	{
		int i, size;
		unsigned char rgba[4];
		unsigned char packet_header;


		int j = 0;
		while (j < image.getWidth() * image.getHeight() * 4)
		{
			/* read first byte */
			packet_header = (unsigned char)fgetc (fp);
			size = 1 + (packet_header & 0x7f);

			if (packet_header & 0x80)
			{
				/* run-length packet */
				fread (rgba, sizeof (unsigned char), 4, fp);

				for (i = 0; i < size; ++i, j = j + 4)
				{
					float blue = rgba[2]/ float(255);
					float green = rgba[1]/ float(255);
					float red = rgba[0]/ float(255);
					float alpha = rgba[3]/ float(255);
					Vec4f color(red,green,blue,alpha);
					image.getData().push_back(color);
				}
			}
			else
			{
				/* non run-length packet */
				for (i = 0; i < size; ++i, j = j + 4)
				{
					float blue = (unsigned char)fgetc (fp)/ float(255);
					float green = (unsigned char)fgetc (fp)/ float(255);
					float red = (unsigned char)fgetc (fp)/ float(255);
					float alpha = (unsigned char)fgetc (fp)/ float(255);
					Vec4f color(red,green,blue,alpha);
					image.getData().push_back(color);
				}
			}
		}
	}


	void
		ReadTGAgray8bitsRLE (FILE *fp, Image& image)
	{
		int i, size;
		float color;
		unsigned char packet_header;

		int j = 0;
		while (j < image.getWidth() * image.getHeight())
		{
			/* read first byte */
			packet_header = (unsigned char)fgetc (fp);
			size = 1 + (packet_header & 0x7f);

			if (packet_header & 0x80)
			{
				/* run-length packet */
				color = (float)fgetc (fp)/ float(255);

				for (i = 0; i < size; ++i, j++)
				{

					Vec4f colors(color,color,color,1);
					image.getData().push_back(colors);
				}
			}
			else
			{
				/* non run-length packet */
				for (i = 0; i < size; ++i, j++)
				{
					float color = (float)fgetc (fp)/ float(255);
					Vec4f colors(color,color,color,1);
					image.getData().push_back(colors);
				}
			}
		}
	}


	void
		ReadTGAgray16bitsRLE (FILE *fp, Image& image)
	{
		int i, size;
		float color, alpha;
		unsigned char packet_header;

		int j = 0;
		while (j < image.getWidth() * image.getHeight() * 2)
		{
			/* read first byte */
			packet_header = (unsigned char)fgetc (fp);
			size = 1 + (packet_header & 0x7f);

			if (packet_header & 0x80)
			{
				/* run-length packet */
				color = (float)fgetc (fp)/ float(255);
				alpha = (unsigned char)fgetc (fp)/ float(255);

				for (i = 0; i < size; ++i, j += 2)
				{
					Vec4f colors(color,color,color,alpha);
					image.getData().push_back(colors);
				}
			}
			else
			{
				/* non run-length packet */
				for (i = 0; i < size; ++i, j += 2)
				{
					float color = (float)fgetc (fp)/ float(255);
					float alpha = (float)fgetc (fp)/ float(255);
					Vec4f colors(color,color,color,alpha);
					image.getData().push_back(colors);
				}
			}
		}
	}


	bool TGAImageLoader::loadImageFromFile(const std::string& filename, Image& image)
	{
		FILE *fp;
		tga_header_t header;
		unsigned char *colormap = NULL;

		fp = fopen (filename.c_str(), "rb");
		if (!fp)
		{
			fprintf (stderr, "error: couldn't open \"%s\"!\n", filename.c_str());
			return false;
		}

		/* read header */
		fread (&header, sizeof (tga_header_t), 1, fp);

		fseek (fp, header.id_lenght, SEEK_CUR);

		/*read into image info*/
		image.setFileName(filename);
		image.setWidth(header.width);
		image.setHeight(header.height);

		/* read color map */
		if (header.colormap_type)
		{
			/* NOTE: color map is stored in BGR format */
			colormap = (unsigned char *)malloc (sizeof (unsigned char)
				* header.cm_length * (header.cm_size >> 3));
			fread (colormap, sizeof (unsigned char), header.cm_length
				* (header.cm_size >> 3), fp);
		}

		/* read image data */
		switch (header.image_type)
		{
		case 0:
			/* no data */
			break;

		case 1:
			/* uncompressed 8 bits color index */
			ReadTGA8bits (fp, colormap, image);
			break;

		case 2:
			/* uncompressed 16-24-32 bits */
			switch (header.pixel_depth)
			{
			case 16:
				ReadTGA16bits (fp, image);
				break;

			case 24:
				ReadTGA24bits (fp, image);
				break;

			case 32:
				ReadTGA32bits (fp, image);
				break;
			}

			break;

		case 3:
			/* uncompressed 8 or 16 bits grayscale */
			if (header.pixel_depth == 8)
				ReadTGAgray8bits (fp, image);
			else /* 16 */
				ReadTGAgray16bits (fp, image);

			break;

		case 9:
			/* RLE compressed 8 bits color index */
			ReadTGA8bitsRLE (fp, colormap, image);
			break;

		case 10:
			/* RLE compressed 16-24-32 bits */
			switch (header.pixel_depth)
			{
			case 16:
				ReadTGA16bitsRLE (fp, image);
				break;

			case 24:
				ReadTGA24bitsRLE (fp, image);
				break;

			case 32:
				ReadTGA32bitsRLE (fp, image);
				break;
			}

			break;

		case 11:
			/* RLE compressed 8 or 16 bits grayscale */
			if (header.pixel_depth == 8)
				ReadTGAgray8bitsRLE (fp, image);
			else /* 16 */
				ReadTGAgray16bitsRLE (fp, image);

			break;

		default:
			/* image type is not correct */
			fprintf (stderr, "error: unknown TGA image type %i!\n", header.image_type);
			return false;
			break;
		}

		/* no longer need colormap data */
		if (colormap)
			free (colormap);

		fclose (fp);
		return true;
	}

}
