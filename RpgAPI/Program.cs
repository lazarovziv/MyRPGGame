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


            //API End Points


            //Getting all of the Enemies Entities
            app.MapGet("api/get/enemies", async (IGameEntitiesRepo repo, IMapper mapper) =>
            {
                var enemies = await repo.GetAllNPCEnemies();

                if (enemies == null)
                    throw new ArgumentNullException(nameof(enemies));

                return Results.Ok(mapper.Map<IEnumerable<GameEntityReadDto>>(enemies));
            });


            //Getting a Player by Id
            app.MapGet("api/get/players/{id}", async (IGameEntitiesRepo repo, IMapper mapper, [FromRoute] long id) =>
            {
                var player = await repo.GetPlayerEntityById(id);

                if(player == null)
                    throw new ArgumentNullException(nameof(player));
                
                return Results.Ok(mapper.Map<GameEntityReadDto>(player));
            });


            //Getting a Enemy by Id
            app.MapGet("api/get/enemies/{id}", async (IGameEntitiesRepo repo, IMapper mapper, [FromRoute] int id) =>
            {
                var enemy = await repo.GetEnemyEntityById(id);

                if (enemy == null)
                    throw new ArgumentNullException(nameof(enemy));

                return Results.Ok(mapper.Map<GameEntityReadDto>(enemy));
            });

            app.Run();
        }
    }
}