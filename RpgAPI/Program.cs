using AutoMapper;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Data.SqlClient;
using Microsoft.EntityFrameworkCore;
using RpgAPI.Data_Access;
using RpgAPI.Dtos.ReadDtos;
using RpgAPI.Repository;

namespace RpgAPI
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            // Add services to the container.
            builder.Services.AddAuthorization();

            //Get the Connection string from the config file
            var sqlConBuilder = new SqlConnectionStringBuilder();
            sqlConBuilder.ConnectionString = builder.Configuration.GetConnectionString("default");

            //Connect to SQL Server with our Connection String
            builder.Services.AddDbContext<RPGGameDBContext>(opt => opt.UseSqlServer(sqlConBuilder.ConnectionString));

            builder.Services.AddScoped<IGameEntitiesRepo, GameEntitiesRepo>();

            builder.Services.AddAutoMapper(AppDomain.CurrentDomain.GetAssemblies());

            var app = builder.Build();

            // Configure the HTTP request pipeline.

            app.UseHttpsRedirection();

            app.UseAuthorization();

            //Getting all of the Game Entities
            app.MapGet("api/gameentities", async (IGameEntitiesRepo repo, IMapper mapper) =>
            {
                var gameEntities = await repo.GetAllGameEntities();
                return Results.Ok(mapper.Map<IEnumerable<GameEntityReadDto>>(gameEntities));
            });

            //Getting a Game Entity by Id
            app.MapGet("api/gameentities/{id}", async (IGameEntitiesRepo repo, IMapper mapper,[FromRoute] long id) =>
            {
                var gameEntity = await repo.GetGameEntityById(id);

                if(gameEntity != null)
                    return Results.Ok(mapper.Map<GameEntityReadDto>(gameEntity));

                return Results.NotFound();
            });

            app.Run();
        }
    }
}