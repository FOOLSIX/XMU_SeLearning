﻿using Microsoft.AspNetCore.Mvc;
using Xmu.Stores.Models;
using Xmu.Stores.Services;

namespace UserStores.Controllers
{
    [Route("~/api/user/[action]")]
    public class UserController : Controller
    {
        public ActionResult AddUser(User user)
        {
            IUserStore us = HttpContext.RequestServices.GetRequiredService<IUserStore>();
            var (success, msg, data) = us.AddUser(user.UserName, user.Password);
            return Ok(new
            {
                success,
                msg,
                data
            });
        }

        public ActionResult GetUsers([FromBody]PagableUserSearcher searcher)
        {
            UserStore us = (HttpContext.RequestServices.GetRequiredService<IUserStore>() as UserStore)!;

            IEnumerable<User> query = us.AllUsers.Select(e => e);
            if (!string.IsNullOrWhiteSpace(searcher.name))
            {
                query = query.Where(e => e.UserName.Contains(searcher.name));
            }

            List<User> data = query.Skip((searcher.page - 1) * searcher.pageSize).Take(searcher.pageSize).ToList();
            int totalRow = us.AllUsers.Count;

            return Ok(new
            {
                success = true,
                msg = "",
                searcher.page,
                searcher.pageSize,
                totalRow,
                data
            });
        }

        public ActionResult GetUser([FromBody]PagableUserSearcher searcher)
        {
            UserStore us = (HttpContext.RequestServices.GetRequiredService<IUserStore>() as UserStore)!;

            User? data = us.AllUsers.FirstOrDefault(e => e.ID == searcher.id);
            if (data == null)
            {
                return Ok(new
                {
                    success = false,
                    msg = "找不到与ID对应的用户",
                    searcher.id
                });
            }

            return Ok(new
            {
                success = true,
                msg = "",
                data
            });
        }
    }

    public record PagableUserSearcher(string name, Guid id, int page = 1, int pageSize = 10);
}
//  Model - View(数据展示给最终用户) - Controller
// http://localhost:5286/user/adduser?UserName=un&Password=pp&Avatar
// http://localhost:5286/user/getUser-value