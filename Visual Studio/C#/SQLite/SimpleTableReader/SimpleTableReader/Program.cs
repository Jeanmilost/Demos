/****************************************************************************
 * ==> SimpleTableReader ---------------------------------------------------*
 ****************************************************************************
 * Description: A simple SQLite example which opens a SQLite database and   *
 *              reads the cells contained in the column of a table.         *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - C# demos                                                   *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sub-license, and/or sell copies of the Software, and to      *
 * permit persons to whom the Software is furnished to do so, subject to    *
 * the following conditions:                                                *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     *
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     *
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        *
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   *
 ****************************************************************************/

using Microsoft.Data.Sqlite;

// NOTE the SQLite NuGet package need to be installed in order to execute this demo code. Please use the below command:
// Install-Package Microsoft.Data.Sqlite

class Program
{
    /// <summary>
    /// Get cells from a table.
    /// </summary>
    /// <param name="fileName">Database file name.</param>
    /// <param name="column">Column name from which the cells should be get.</param>
    /// <param name="table">Table name from which the cells should be get.</param>
    /// <returns>Cells, empty list on error.</returns>
    public static List<string> GetCells(string fileName, string column, string table, string filter = "")
    {
        List<string> cells = new List<string>();

        // database file exists?
        if (!File.Exists(fileName))
        {
            Console.WriteLine("Database file not found - " + fileName);
            return cells;
        }

        // define the connection string pointing to the database file
        string connectionString = "Data Source=" + fileName + ";";

        // connect to the database
        using (SqliteConnection connection = new SqliteConnection(connectionString))
        {
            connection.Open();

            string query;

            // build the SQLite query
            if (filter == "")
                query = "SELECT " + column + " FROM " + table + ";";
            else
                query = "SELECT " + column + " FROM " + table + " WHERE " + filter + ";";

            // create the command object
            using (SqliteCommand command = new SqliteCommand(query, connection))
                // execute the query and read the data rows
                using (SqliteDataReader reader = command.ExecuteReader())
                    while (reader.Read())
                        // GetString(0) safely reads the first (and only) column selected
                        if (!reader.IsDBNull(0))
                            cells.Add(reader.GetString(0));
        }

        return cells;
    }

    /// <summary>
    /// Main application entry point.
    /// </summary>
    /// <param name="args">argument list.</param>
    static void Main(string[] args)
    {
        Console.WriteLine("Executing SQLite query...\n");

        try
        {
            // open the database and get the apple ID cells from account table
            List<string> names = GetCells("..\\..\\..\\..\\Database\\MediaLibrary.sqlitedb", "apple_id", "account");

            // print out the fetched results
            Console.WriteLine($"Retrieved {names.Count} names:");

            foreach (string name in names)
                Console.WriteLine($"- {name}");

            Console.WriteLine("\nNow apply a filter...\n");

            // get the apple ID cells from account table, but also apply a filter
            names = GetCells("..\\..\\..\\..\\Database\\MediaLibrary.sqlitedb", "apple_id", "account", "dsid=1014958110");

            // print out the fetched results
            Console.WriteLine($"Retrieved {names.Count} names:");

            foreach (string name in names)
                Console.WriteLine($"- {name}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"An error occurred: {ex.Message}");
        }

        Console.WriteLine("\nPress any key to close.");
        Console.ReadKey();
    }
}
