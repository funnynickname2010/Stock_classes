#include "Stock.h"

bool Stock::UploadToFile()
{
	std::ofstream myfile(file_name); //Erases the contents!!
	int res;

	if (myfile.good())
	{
		res = 1;

		myfile << size_filled << endl;

		for (int i = 0; i < size_filled; i++)
		{
			myfile << stock_array[i].name << ' ' << stock_array[i].quantity << ' ' << stock_array[i].price << endl;
		}
	}
	else
	{
		res = 0;
	}

	myfile.close();
	return res;
}

int Stock::DownloadFromFile()
{
	//File structure:
	//size_filled
	//stock_array[i].name stock_array[i].quantity stock_array[i].price


	std::fstream myfile(file_name);

	int res;

	if (myfile.good() && (myfile.peek() != std::fstream::traits_type::eof()))
	{
		res = 1;

		myfile >> size_filled;
		myfile.ignore();

		if (size_filled > size_allocated)
		{
			std::cout << "Not enough stock capacity to save all the products, some data may be lost\n";
			size_filled = size_allocated;
		}

		for (int i = 0; (i < size_filled) && (i < size_allocated); i++)
		{
			myfile >> stock_array[i].name; //Maybe it works, I'm not sure

			myfile >> stock_array[i].quantity;
			myfile >> stock_array[i].price;
			myfile.ignore();

			for (int j = 0; stock_array[i].name[j - 1] != '\0'; j++)
			{
				if (stock_array[i].name[j] == '\0')
				{
					stock_array[i].name_len_filled = j;
				}
			}
		}
	}
	else if (myfile.good())
	{
		res = -1;
	}
	else if (!myfile.good())
	{
		res = 0;
	}

	myfile.close();
	return res; //Really it's fine
}

Stock::Stock()
{
	stock_array = new Product[STOCK_CAPACITY];
	size_allocated = STOCK_CAPACITY;
	size_filled = 0;
	file_name = FILE_NAME_STOCK;
}

Stock::~Stock()
{
	delete[] stock_array;
}

void Stock::CheckOut()
{
	double sum_price = 0;
	
	cout << "\n\n--------------------CHECKOUT--------------------\n";
	cout << "index" << setw(SETW_PARAM) << "name" << setw(SETW_PARAM) << "quantity" << setw(SETW_PARAM) << "price" << endl;

	for (int i = 0; (i < size_filled) && (i < size_allocated); i++)
	{
		std::cout << i << "    " << setw(SETW_PARAM) << stock_array[i].name << setw(SETW_PARAM) << stock_array[i].quantity << setw(SETW_PARAM) << stock_array[i].price << endl;
		sum_price += stock_array[i].quantity * stock_array[i].price;
	}

	cout << "Total cost: " << sum_price;
	cout << "\n\n";
}

int Stock::Search(char* myname)
{
	int res = -1;

	for (int i = 0; (i < size_filled) && (i < size_allocated); i++)
	{
		if (stock_array[i].LexicographicallyCompareNames(myname) == -1)
		{
			res = i;
		}
	}

	return res;
}

void Stock::ChangeQuantity(int index, int delta)
{
	if ((delta < 0) && (stock_array[index].quantity >= -delta))
	{
		stock_array[index].quantity += delta;
	}
	else if (delta > 0)
	{
		stock_array[index].quantity += delta;
	}

	if (stock_array[index].quantity == 0)
	{
		RemoveProduct(index);
	}
}

void Stock::RemoveProduct(int index)
{
	for (int i = index; i < size_filled - 1; i++)
	{
		stock_array[i] = stock_array[i + 1];
	}

	size_filled--;
}

void Stock::AddNewProduct(Product myProduct)
{
	int index_to_insert_at = -1;

	for (int i = 0; i < size_filled; i++)
	{
		if (myProduct.LexicographicallyCompareNames(stock_array[i]) == 1)
		{
			index_to_insert_at = i;
		}
		else if (myProduct.LexicographicallyCompareNames(stock_array[i]) == -1)
		{
			stock_array[i].quantity += myProduct.quantity;
			return;
		}
	}

	if (index_to_insert_at == -1)
	{
		index_to_insert_at = size_filled;
	}

	if (size_allocated > size_filled)
	{
		for (int i = size_filled; i > index_to_insert_at; i--)
		{
			stock_array[i] = stock_array[i - 1];
		}

		stock_array[index_to_insert_at] = myProduct;
		size_filled += 1;
	}
	else
	{
		cout << "No free space in stock." << endl;
	}
}
