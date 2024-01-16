Webshop
In this task you need to implement a simplified grocery Webshop.

Create a structure that contains a set of products and their number. A product has a name, price and quantity. Create two instances of this struct. The first represents the webshop store and the other represents a cart.

Print a menu for the user that contains the following items:

Load webshop store from a file\ This operation loads the content of the store from a text file. The filename should be read from the keyboard. The file contains the product name, price and quantity in the following format:
Apple 100 42
Orange 130 59
Raspberry 231 17
...
Write the content of the store to a file\ Read a filename from the user and write the content of the store to that file using the file format above.
Insert an item to cart\ The cart contains the items that a customer wants to buy. Read a product name and the quantity from the keyboard and put this information to the cart. The price of the product should be fetched from the store.
Get the price of cart\ This operation prints the sum of the products' prices in the cart.
Buy the products\ When this option is chosen, then the content of the cart is subtracted from the store. If a certain amount of product is not available in the store then print an error message to the user which tells how many products are missing. For example if the user needs 10 apples and the store contains only 8, then the following message should be printed: "2 apples are missing". In this situation, subtract the available amount of products from the store. Make sure that a product can't have negative quantity in the store.
Exit the program\ This option finishes the program execution.
The program keeps displaying this menu to the user until they choose the exit option.

The content of the store and the cart should be stored in an array on the heap memory. In case the array is full, you should allocate a bigger array and remove the previous one.

Implement the operations in separate function. For example, the inserter function gets a cart or a store, and a product as parameter. This product is inserted to the given cart or store. If the product is already inserted, then the quantity should be increased instead of inserting the product twice.

Separate the program to multiple translation units. The first translation unit contains the program control and the other contains the implemented types and their operations.
